#include "Arduino.h"
#include <FastLED.h>
#include <ESP32Encoder.h>
#include "LedMeter.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include "MotorCommands.h"
#include "I2CAnything.h"
#include "Payload.h"

#define ROTARY_ENCODER_A_PIN 14
#define ROTARY_ENCODER_B_PIN 13
#define ROTARY_ENCODER_BUTTON_PIN 25
#define LED_PIN 27
#define NUM_LEDS 13
#define I2C_ADDRESS 0x3C
#define DEBUG 1

CRGB leds[NUM_LEDS];
SSD1306AsciiWire oled;
ESP32Encoder encoder;
LedRange payloadRange [1] = {  { 0, 12 } } ; 
LedMeter payloadMeter = LedMeter(leds,payloadRange,1,CRGB::Blue, CRGB::Black);
GameData gameData { 0 };
Payload payload;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  setupLEDs();
  setupEncoder();
  setupOLED();
}

void setupLEDs(){
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  payloadMeter.setMaxValue(NUM_LEDS);  
}

void setupEncoder(){
  encoder.clearCount();
  encoder.attachSingleEdge(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN);  
}

void setupOLED(){
  Wire.setClock(400000L);  
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.displayRemap(false);
  oled.setFont(Adafruit5x7);
  oled.clear();
  oled.println("BattlePoint v1.0"); 
  oled.println("Starting..."); 
}

void loop() {
  payload.update();
  payloadMeter.update();
  handleCommand(payload.lastCommand);
  
  #if DEBUG
  updateSerial();
  #endif
  
  updateDisplay();
  updateLEDs();
  
  payloadMeter.setValue(encoder.getCount());

  FastLED.delay(100);  
}

void handleCommand(MotorCommand mc){
    Wire.beginTransmission (MOTOR_CONTROLLER_ADDRESS );
    I2C_writeAnything (mc);
    Wire.endTransmission ();  
}

void updateLEDs(){
  payloadMeter.update();
  FastLED.show();
}

void updateDisplay(){
  int gameTimeSec = (millis() - gameData.startTime)/1000;
  char SPACE = ' ';
  oled.setCursor(0,1);
  oled.print("Time: ");
  oled.print(gameTimeSec);
  oled.print(" s");
  oled.clearToEOL();
  
  oled.setCursor(0,2);
  oled.print("BTNS:");
  oled.print(SPACE);
  oled.print(payload.fwd_btn_1);
  oled.print(SPACE);
  oled.print(payload.fwd_btn_2);
  oled.print(SPACE);
  oled.print(payload.fwd_btn_3);
  oled.print(SPACE);
  oled.print(payload.bwd_btn_1);
  oled.clearToEOL();
  oled.setCursor(0,3);
  oled.print("SENS: L:");
  oled.print(payload.left_sensor);
  oled.print(" R:");   
  oled.print(payload.right_sensor);
  oled.print(SPACE);
  oled.clearToEOL();
  oled.setCursor(0,4);
  oled.print("MTR: L ");  
  oled.print(payload.lastCommand.leftVelocity);
  oled.print(" R: ");
  oled.print(payload.lastCommand.rightVelocity);
  oled.clearToEOL();
}

void updateSerial(){
   char SPACE = ' ';
   Serial.print("B:");
   Serial.print(SPACE);
   Serial.print(payload.fwd_btn_1);
   Serial.print(SPACE);
   Serial.print(payload.fwd_btn_2);
   Serial.print(SPACE);
   Serial.print(payload.fwd_btn_3);
   Serial.print(SPACE);
   Serial.print(payload.bwd_btn_1);
   Serial.print(SPACE);
   Serial.print("L:");
   Serial.print(payload.left_sensor);
   Serial.print(SPACE);
   Serial.print("R:");   
   Serial.print(payload.right_sensor);
   Serial.print(SPACE);     
   Serial.print("LM:");
   Serial.print(payload.lastCommand.leftVelocity);
   Serial.print(SPACE);   
   Serial.print("RM:");
   Serial.print(payload.lastCommand.rightVelocity);
   Serial.println(SPACE);     
}
