#include <Adafruit_NeoPixel.h>
#include <TinyWireM.h>
#include "TinyRTClib.h"

#define PIN 1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(13, PIN, NEO_GRB + NEO_KHZ800);
RTC_DS1307 RTC;

unsigned int tick;
unsigned int lastSecond, lastMinute, lastHour;

void setup() {
  strip.begin();
  strip.show(); 
  strip.setBrightness(64); // quarter bright
  pinMode(5,INPUT_PULLUP);

  TinyWireM.begin();
  RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop() {
 
  DateTime now = RTC.now();

  // every second (and restart)  
  if (lastSecond != now.second()) {
    lastSecond=now.second();
    tick++;

    // every hour (and restart)
    if (lastHour != now.hour()) {
      lastHour=now.hour();
        for(unsigned long x=0;x<256*5;x++) {
        rainbowWipe(x);
        strip.show();
        delay(10);   
      }
    }

    // every minute (and restart)
    if (lastMinute != now.minute()) {
      lastMinute=now.minute();

      for(unsigned long x=0;x<256;x++) {
        rainbowWipe(x);
        strip.show();
        delay(10);      
      }
    }

    // Fill the background
    rainbowWipe((tick*10)%256);

    // set the time
    clockUnset(1,now.minute()%10);
    clockUnset(2,now.minute()/10);

    // 12 hour clock
    if (now.hour() > 12) {
      clockUnset(3,(now.hour()-12)%10);
      clockUnset(4,(now.hour()-12)/10);
    } else {
      clockUnset(3,now.hour()%10);
      clockUnset(4,now.hour()/10);
    }
    
    strip.show();
  }
}

void rainbowWipe(unsigned int pos) {
   for (unsigned int i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + pos) & 255));
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

// Unset pixels for specific clock digit
void clockUnset(int unsigned digit, unsigned int value) {
  unsigned int first, last, num;
  switch(digit) {
    case 1:
      first=0;
      num=4;   
      break;
    case 2:
      first=4;
      num=3;   
      break;
    case 3:
      first=7;
      num=4;   
      break;
    case 4:
      first=11;
      num=2;   
      break;
  }
  for (unsigned int i=0; i < num; i++) {
    if (!(value&(1<<i)))
      strip.setPixelColor(i+first, 0);
  }
}

