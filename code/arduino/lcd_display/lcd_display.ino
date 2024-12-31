#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
String lastCommand = "";
String messageToPrint = "";

int startIndex = 0; // Start index of the substring
unsigned long lastUpdateTime = 0; // Track time for updates
const int scrollInterval = 500; // Interval between updates in milliseconds

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    if (input != lastCommand) {
      lastCommand = input;
      messageToPrint = "        " + lastCommand + "        ";
    }
  }

  if (millis() - lastUpdateTime >= scrollInterval) {
    lastUpdateTime = millis();

    // Clear the LCD and print the substring
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(messageToPrint.substring(startIndex, startIndex + 16));

    // Update the start index for the next part
    startIndex++;

    // Reset the start index if we've reached the end of the string
    if (startIndex + 16 > messageToPrint.length()) {
      startIndex = 0;
    }
  }
}
