from robot_handler import RobotHandler
from flask import Flask, request, jsonify, render_template
import atexit, time , serial

app = Flask(__name__)

robot_handler = None

def init_robot_handler():
    max_retries = 3
    retry_delay = 2

    for attempt in range(max_retries):
        try:
            global robot_handler
            robot_handler = RobotHandler()
            return True
        except serial.SerialException as e:
            print(f"Attempt {attempt + 1}/{max_retries}: Failed to initialize ports: {e}")
            if attempt < max_retries - 1:
                time.sleep(retry_delay)
    return False

def cleanup():
    if robot_handler:
        robot_handler.close()

atexit.register(cleanup)

@app.route('/')
def index():
    global robot_handler
    if robot_handler is None:
        if not init_robot_handler():
            return "Robot handler not initialized", 503
    return render_template('index.html')

@app.route('/send_command', methods=['POST'])
def send_command():
    try:
        data = request.get_json()
        if not data:
            return jsonify({
                "status": "error",
                "message": "No JSON data received"
            }), 400

        command = data.get('command')
        if not command:
            return jsonify({
                "status": "error",
                "message": "No command specified"
            }), 400

        # Validate command is one of the allowed commands
        allowed_commands = ["SHAKE HANDS", "GRAB STRAIGHT", "SAY YES", "SAY NO"]
        if command not in allowed_commands:
            return jsonify({
                "status": "error",
                "message": f"Invalid command. Allowed commands: {allowed_commands}"
            }), 400

        # Log the command for debugging
        print(f"Executing command: {command}")
        
        # Send command to robot
        robot_handler.send_command(command)
        
        return jsonify({
            "status": "success",
            "message": f"Command '{command}' executed successfully"
        })

    except Exception as e:
        print(f"Error in send_command: {str(e)}")
        return jsonify({
            "status": "error",
            "message": f"Server error: {str(e)}"
        }), 500

@app.route('/send_chat', methods=['POST'])
def send_chat():
    data = request.get_json()
    prompt = data.get('prompt')
    response = robot_handler.get_response(prompt)
    print(f"Response received: {response}") 
    robot_handler.send_command(response['Command'])
    robot_handler.print_on_lcd(response['Text'])
    return jsonify({"text": response['Text']})

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, debug=True)