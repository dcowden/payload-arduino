#include <Arduino.h>
#include <FastLED.h>


#ifndef __INC_LEDMETER_H
#define __INC_LEDMETER_H

struct LedRange {
    //zero based
    uint8_t startIndex;
    uint8_t endIndex;
};


class Meter {
  public:
    Meter ();
    void setValue(int value );
    void setMaxValue(int value);
    void setToMax();
    void setToMin();
    int getValue();
    int getMaxValue();
    virtual void update() = 0;
    virtual void setColors(CRGB fg, CRGB bg)= 0;
  private:
    int value;
    int maxValue;
};

class LedMeter : public Meter{

  //a meter which has two colors, and can represent a value between two boundaries
  public:
    LedMeter (CRGB* leds, LedRange* ranges, uint8_t new_ranges_cnt,CRGB new_fgcolor, CRGB new_bgcolor );    
    virtual void update();
    virtual void setColors(CRGB fg, CRGB bg);
  private:
    void setFgColor(CRGB color);    
    void setBgColor(CRGB color);  
    CRGB* leds;
    LedRange* ranges;
    uint8_t ranges_cnt;    
    CRGB fgColor;
    CRGB bgColor;   
    void updateRange(LedRange* range);
};




#endif
