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
#define minNumberOfExposures 1
#define maxNumberOfExposures 100 //absolute max 255
#define waitingTimeBetweenExposures 2000

//OLED
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Global Variables
bool isExposing = false;
byte numberOfExposures = 30;
unsigned long exposureTime = 120000;
byte settingSelected = 1;
byte exposureCounter = 0;
unsigned long startedSessionAtMillis = 0;
bool abortSession = false;
unsigned long startMillis; //used for calculation dime spent in exposing and waiting

void increaseExposure(void)
{
  exposureTime += 1000;           //increase 1s exposure time
  if (exposureTime > maxExposure) //roll over to defined minExposure
  {
    exposureTime = minExposure;
  }
}

void decreaseExposure(void)
{
  exposureTime -= 1000;           //decrease 1s exposure time
  if (exposureTime < minExposure) //roll over to defined maxExposure
  {
    exposureTime = maxExposure;
  }
}

void increaseNumberOfExposures(void)
{
  numberOfExposures++;                          //increase number of exposures
  if (numberOfExposures > maxNumberOfExposures) //roll over to defined minNumberOfExposures
  {
    numberOfExposures = minNumberOfExposures;
  }
}

void decreaseNumberOfExposures(void)
{
  numberOfExposures--;                          //decrease number of exposures
  if (numberOfExposures < minNumberOfExposures) //roll over to defined maxNumberOfExposures
  {
    numberOfExposures = maxNumberOfExposures;
  }
}

void nextSetting(void)
{
  settingSelected++;       //increase Setting
  if (settingSelected > 2) //Roll over
  {
    settingSelected = 1;
  }
}

void startSession(void)
{
  exposureCounter = 0;
  isExposing = true;
  startedSessionAtMillis = millis();
  exposureCounter = 1;
  abortSession = false;
}

void displaySettingsScreen(void)
{
  display.clearDisplay();

  //Exposure Time
  display.setCursor(0, 5);
  display.setTextSize(1);
  display.print(F("ExpoTime:  "));
  display.setCursor(70, 5);
  display.setTextSize(2);
  display.print(exposureTime / 1000);
  display.print(F("s"));
  if (settingSelected == 1) // highlight if selected
  {
    display.drawRect(65, 0, 60, 24, SSD1306_WHITE);
  }

  //Number of Exposures
  display.setCursor(0, 25);
  display.setTextSize(1);
  display.print(F("Exposures:  "));
  display.setCursor(70, 25);
  display.setTextSize(2);
  display.print(numberOfExposures);
  if (settingSelected == 2) // highlight if selected
  {
    display.drawRect(65, 20, 60, 24, SSD1306_WHITE);
  }

  display.display();
}

void handleButtonsInSettings()
{
  int buttonvalue = analogRead(pinAnalogButtonInput);

  if (buttonvalue <= valueButtonUp + buttonTolerance && buttonvalue >= valueButtonUp - buttonTolerance) //UP
  {
    if (settingSelected == 1)
    {
      increaseExposure();
    }
    else if (settingSelected == 2)
    {
      increaseNumberOfExposures();
    }
  }

  if (buttonvalue <= valueButtonDown + buttonTolerance && buttonvalue >= valueButtonDown - buttonTolerance) //DOWN
  {
    if (settingSelected == 1)
    {
      decreaseExposure();
    }
    else if (settingSelected == 2)
    {
      decreaseNumberOfExposures();
    }
  }

  if (buttonvalue <= valueButtonNext + buttonTolerance && buttonvalue >= valueButtonNext - buttonTolerance) //NEXT
  {
    nextSetting();
  }

  if (buttonvalue <= valueButtonStart + buttonTolerance && buttonvalue >= valueButtonStart - buttonTolerance) //START
  {
    startSession();
  }
}

void handleButtonsInSession()
{
  int buttonvalue = analogRead(pinAnalogButtonInput);

  if (buttonvalue <= valueButtonUp + buttonTolerance && buttonvalue >= valueButtonUp - buttonTolerance) //UP
  {
  }

  if (buttonvalue <= valueButtonDown + buttonTolerance && buttonvalue >= valueButtonDown - buttonTolerance) //DOWN
  {
  }

  if (buttonvalue <= valueButtonNext + buttonTolerance && buttonvalue >= valueButtonNext - buttonTolerance) //NEXT
  {
    abortSession = true;
  }

  if (buttonvalue <= valueButtonStart + buttonTolerance && buttonvalue >= valueButtonStart - buttonTolerance) //START
  {
    //
  }
}

void displayExposureScreen(bool waiting)
{
  display.clearDisplay();

  //Total Time
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print(F("Total Time:  "));
  display.print((millis() - startedSessionAtMillis) / 1000);
  display.print(F("s"));

  //Exposure Number
  display.setCursor(0, 10);
  display.setTextSize(1);
  display.print(F("Exposure Number:  "));
  display.print(exposureCounter);

  //Current Exposure Time
  display.setCursor(0, 20);
  display.setTextSize(1);
  display.print(F("Time Expo:  "));
  if (waiting)
  {
    display.print(F("waiting"));
  }
  else
  {
    int timeSpentInExposure = (exposureTime - (startMillis + exposureTime - millis())) / 1000;
    display.print(timeSpentInExposure);
  }

  //Abort Session
  if (abortSession)
  {
    display.setCursor(90, 54);
    display.setTextSize(1);
    display.print(F("ABORT"));
  }

  //Settings
  display.setCursor(0, 44);
  display.setTextSize(1);
  display.print(F("ExpoTime:  "));
  display.print(exposureTime / 1000);
  display.print(F("s"));
  display.setCursor(0, 54);
  display.setTextSize(1);
  display.print(F("Exposures:  "));
  display.print(numberOfExposures);

  display.display();
}

void setup()
{
  Serial.begin(9600);

  pinMode(pinRelayTrigger, OUTPUT);
  digitalWrite(pinRelayTrigger, LOW);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true);                 // Use full 256 char 'Code Page 437' font

  delay(200); //Sanity delay
}

void loop()
{

  while (!isExposing) //Settings
  {
    displaySettingsScreen();
    handleButtonsInSettings();
    delay(100); //tick for settings is 100ms
  }

  while (isExposing) //Exposing
  {

    startMillis = millis();
    while (startMillis + exposureTime > millis())
    {
      digitalWrite(pinRelayTrigger, HIGH);
      displayExposureScreen(false);
      handleButtonsInSession();
    }

    startMillis = millis();
    while (startMillis + waitingTimeBetweenExposures > millis()) //Waiting
    {
      digitalWrite(pinRelayTrigger, LOW);
      displayExposureScreen(true);
      handleButtonsInSession();
    }

    exposureCounter++;
    if (exposureCounter > numberOfExposures || abortSession)
    {
      isExposing = false;
    }
  }
}