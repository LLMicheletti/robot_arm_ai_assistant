import google.generativeai as genai
from dotenv import load_dotenv
import os	
import serial

class RobotHandler:
    def __init__(self):
        self.robot_arm_board = None
        self.lcd_board = None
        self.initialize_ports()
        load_dotenv()
        api_key = os.getenv('GEMINI_API_KEY')
        genai.configure(api_key=api_key)
        model = genai.GenerativeModel("gemini-1.5-flash")
        self.chat = model.start_chat(
            history=[
                {"role": "user", "parts": "Hello, answer to my messages with Command: a command to give to a robot arm and Text: the full answer to read."},
                {"role": "model", "parts": "Great to meet you. I am a helpful assistant powered by a robot arm: As a robot arm I have to respond with one command among: 'SHAKE HANDS' to say hello or default to provide explanation, 'GRAB STRAIGHT' if asked to grab an object, 'SAY YES' to agree, 'SAY NO' to disagree; as a chatbot I have to provide the full answer in plain text."},
            ]
        )

    def initialize_ports(self):
        try:
            self.robot_arm_board = serial.Serial('COM3', 9600, timeout=1)
            self.lcd_board = serial.Serial('COM4', 9600, timeout=1)
        except serial.SerialException as e:
            print(f"Error initializing ports: {e}")
            self.close()
            raise

    def close(self):
        if self.robot_arm_board and self.robot_arm_board.is_open:
            self.robot_arm_board.close()
        if self.lcd_board and self.lcd_board.is_open:
            self.lcd_board.close()

    def get_response(self, prompt):
        try:
            response = self.chat.send_message(prompt)
            if "Command:" in response.text:
                command_text = response.text.split("Command:")[1].split("\n")[0].strip()
            if "Text:" in response.text:
                full_answer_text = response.text.split("Text:")[1].strip()
            return {"Command": command_text or 'SAY YES', "Text": full_answer_text or 'No response created, please try again'}
        except Exception as e:
            print(f"Error processing response: {str(e)}")
            return {"error": str(e), "Command": None, "Text": None}
        
    def send_command(self, command):
        self.robot_arm_board.write((command + "\n").encode())

    def print_on_lcd(self, text):
        self.lcd_board.write((text + "\n").encode())