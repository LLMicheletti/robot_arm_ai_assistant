# Robot Arm AI Assistant
An enhanced AI assistant, which goes beyond the chat and is able to interact in the physical world with a robot arm and an LCD Display
## HARDWARE
* Keyestudio 4DOF Robot Arm Kit for Arduino DIY KS0198X
* Arduino Uno
* LCD Display
* 10 kohm potentiometer
* 16 jumper wires
## HOW TO USE
* Copy the [code](code/flask_app) for the Flask App in a directory and upload the [sketches](code/arduino) to the boards connected to the robot arm and the lcd display;
* Get a [Gemini API Key](https://aistudio.google.com/app/apikey) and save it in a .env file in the same directory of the Flask app;
* Execute the [app](code/flask_app/main.py) with admin privileges and go to the [port](http://127.0.0.1:5000);
* Chat with your assistant and interact with it!
