#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define pinAnalogButtonInput A0
#define pinRelayTrigger D6

#define valueButtonUp 588
#define valueButtonDown 328
#define valueButtonNext 231
#define valueButtonStart 181
#define buttonTolerance 10

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void handleButtons(int buttonvalue) {
  if (buttonvalue <= valueButtonUp + buttonTolerance && buttonvalue >= valueButtonUp - buttonTolerance){
    display.setCursor(0, 20); 
    display.setTextSize(2);
    display.print(F("UP"));
  }
  if (buttonvalue <= valueButtonDown + buttonTolerance && buttonvalue >= valueButtonDown - buttonTolerance){
    display.setCursor(0, 20); 
    display.setTextSize(2);
    display.print(F("DOWN"));
  }
  if (buttonvalue <= valueButtonNext + buttonTolerance && buttonvalue >= valueButtonNext - buttonTolerance){
    display.setCursor(0, 20); 
    display.setTextSize(2);
    display.print(F("NEXT"));
  }
  if (buttonvalue <= valueButtonStart + buttonTolerance && buttonvalue >= valueButtonStart - buttonTolerance){
    display.setCursor(0, 20); 
    display.setTextSize(2);
    display.print(F("START"));

    digitalWrite(pinRelayTrigger, HIGH);
  }else{
    digitalWrite(pinRelayTrigger, LOW);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(pinRelayTrigger, OUTPUT);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  testdrawchar();      // Draw characters of the default font

  // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
}

void loop() {
  int val = analogRead(pinAnalogButtonInput);
  Serial.println(val);

  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(0, 0); 
  display.print(val);

  display.setCursor(40, 0); 
  display.print(millis());

  handleButtons(val);

  display.display();

  delay(100);
}