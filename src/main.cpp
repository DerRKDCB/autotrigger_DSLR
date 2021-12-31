#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Pin Definitions
#define pinAnalogButtonInput A0
#define pinRelayTrigger D6

//Button Values
#define valueButtonUp 588
#define valueButtonDown 328
#define valueButtonNext 231
#define valueButtonStart 181
#define buttonTolerance 10

//Function Options
#define minExposure 1000
#define maxExposure 300000

//OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Global Variables
long exposuretime = 180000;

void increaseExposure(void){
  exposuretime += 1000;
  if (exposuretime > maxExposure){
    exposuretime = minExposure;
  }
}

void decreaseExposure(void){
  exposuretime -= 1000;
  if (exposuretime < minExposure){
    exposuretime = maxExposure;
  }
}

void handleButtons(int buttonvalue) {

  display.setCursor(0, 40); 
  display.setTextSize(2);

  if (buttonvalue <= valueButtonUp + buttonTolerance && buttonvalue >= valueButtonUp - buttonTolerance){
    display.print(F("UP"));
    increaseExposure();
  }
  if (buttonvalue <= valueButtonDown + buttonTolerance && buttonvalue >= valueButtonDown - buttonTolerance){
    display.print(F("DOWN"));
    decreaseExposure();
  }
  if (buttonvalue <= valueButtonNext + buttonTolerance && buttonvalue >= valueButtonNext - buttonTolerance){
    display.print(F("NEXT"));
  }
  if (buttonvalue <= valueButtonStart + buttonTolerance && buttonvalue >= valueButtonStart - buttonTolerance){
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

  // Clear the buffer
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  delay(200); //Sanity delay
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

  display.setCursor(0, 20); 
  display.print(exposuretime);

  display.display();

  handleButtons(val);

  display.display();

  delay(100);
}