#include "Arduino.h"
#include <FastLED.h>
#include <ESP32Encoder.h>
#include "LedMeter.h"

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

void loop() {
  
  encoderValue = encoder.getCount();
  Serial.print("Val= "); Serial.println(encoderValue);
  timer1Meter.setValue(encoderValue);
  FastLED.show();
  //FastLED.delay(200);
  delay(100);
}
