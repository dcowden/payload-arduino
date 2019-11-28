#include "Arduino.h"
#include <FastLED.h>
#include <ESP32Encoder.h>
#include "LedMeter.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#define ROTARY_ENCODER_A_PIN 14
#define ROTARY_ENCODER_B_PIN 13
#define ROTARY_ENCODER_BUTTON_PIN 25
#define LED_PIN 27
#define NUM_LEDS 13

CRGB leds[NUM_LEDS];

ESP32Encoder encoder;
LedRange timer1Range [1] = {  { 0, 12 } } ; 
LedMeter timer1Meter = LedMeter(leds,timer1Range,1,CRGB::Blue, CRGB::Black);

int encoderValue = 0;

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  encoder.setCount(0);
  encoder.clearCount();
  encoder.attachSingleEdge(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN);
  timer1Meter.setMaxValue(13);
}

//void setupOLED(){
//  Wire.begin();
//  Wire.setClock(400000L);  
//  oled.begin(&Adafruit128x64, I2C_ADDRESS);
//
//  //invert display
//  oled.displayRemap(false);
//  oled.setFont(Adafruit5x7);
//  oled.clear();
//  oled.println("BattlePoint v1.0"); 
//  oled.println("Starting..."); 
//  
//}

void loop() {

//void updateLEDs(Pose p, PayloadCommand pc){
//  mainMeter.update();
//  FastLED.show();
//}

//  Pose p = readPose();
//  PayloadCommand pc = computePayloadCommand(p);
//  handleCommand(pc);
//  #if DEBUG
//  updateSerial(p,pc);
//  #endif
//  updateDisplay(p,pc);
//  updateLEDs(p,pc);
  
  encoderValue = encoder.getCount();
  Serial.print("Val= "); Serial.println(encoderValue);
  timer1Meter.setValue(encoderValue);
  FastLED.show();
  //FastLED.delay(200);
  delay(100);
}

void updateDisplay(Pose p, PayloadCommand pc){
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
  oled.print(p.fwd_btn_1);
  oled.print(SPACE);
  oled.print(p.fwd_btn_2);
  oled.print(SPACE);
  oled.print(p.fwd_btn_3);
  oled.print(SPACE);
  oled.print(p.bwd_btn_1);
  oled.clearToEOL();
  oled.setCursor(0,3);
  oled.print("SENS: L:");
  oled.print(p.left_sensor);
  oled.print(" R:");   
  oled.print(p.right_sensor);
  oled.print(SPACE);
  oled.clearToEOL();
  oled.setCursor(0,4);
  oled.print("MTR: L ");  
  oled.print(pc.leftSpeed);
  oled.print(" R: ");
  oled.print(pc.rightSpeed);
  oled.clearToEOL();
}

void updateSerial(Pose p, PayloadCommand pc){
   char SPACE = ' ';
   Serial.print("B:");
   Serial.print(SPACE);
   Serial.print(p.fwd_btn_1);
   Serial.print(SPACE);
   Serial.print(p.fwd_btn_2);
   Serial.print(SPACE);
   Serial.print(p.fwd_btn_3);
   Serial.print(SPACE);
   Serial.print(p.bwd_btn_1);
   Serial.print(SPACE);
   Serial.print("L:");
   Serial.print(p.left_sensor);
   Serial.print(SPACE);
   Serial.print("R:");   
   Serial.print(p.right_sensor);
   Serial.print(SPACE);     
   Serial.print("LM:");
   Serial.print(pc.leftSpeed);
   Serial.print(SPACE);   
   Serial.print("RM:");
   Serial.print(pc.rightSpeed);
   Serial.println(SPACE);     
}
