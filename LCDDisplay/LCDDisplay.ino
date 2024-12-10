
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

void setup() {
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    // Print a message to the LCD.
    lcd.print("Smart Home Care System");
}

void loop() {
    lcd.noDisplay();    // Turn off 
    delay(500);

    lcd.display();    // Turn on 
    delay(500);
}

