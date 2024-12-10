//============================================================================
//  ImsaiGuy 2024
//  for the ATTINY814
//  uses OLED display connected to pins 8 (SDA) and 9 (SCL)
// also blinks LED
//============================================================================

#define LED 1               // pin number for LED
#define Button 5

#include <Tiny4kOLED.h>     // Driver for I2C 1306 OLED display
//#include "ModernDos.h"    // Extra Font
#include "ModernDos8.h"     // Extra Font
int count = 0;
//============================================================================
void setup() {
  pinMode(LED, OUTPUT);      
  pinMode(Button,INPUT_PULLUP);
  oled.begin();
  //oled.setFont(FONT6X8);
  //oled.setFont(FONT8X16MDOS);
  oled.setFont(FONT8X8MDOS);
  oled.clear();
  oled.on();
  oled.setCursor(0, 0);
  oled.println("ImsaiGuy ");
  oled.println("ATTINY814 ");
  
}

//============================================================================
void loop() {
  digitalWrite(LED, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(50);                      // wait for a second
  digitalWrite(LED, LOW);   // turn the LED off by making the voltage LOW
  delay(50);                      //
  digitalWrite(LED, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(150);                      // wait for a second
  digitalWrite(LED, LOW);   // turn the LED off by making the voltage LOW
  delay(300);                      // wait for a second
  count = count + 1;
  oled.setCursor(0, 10);
  oled.println(count);
  if (digitalRead(Button) == 0) {
    // put stuff here on button press
    count = 0;
    oled.setCursor(0, 10);
    oled.println("        ");
    }

}

//============================================================================
// End of file
//============================================================================