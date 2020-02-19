const int outPin = 0;
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 300 

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 2
CRGB leds[NUM_LEDS];

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
 
void setup() 
{
   Serial.begin(9600);
   LEDS.addLeds<WS2812B,DATA_PIN,GRB>(leds,NUM_LEDS);
  LEDS.setBrightness(84);
}
 
 
void loop() 
{
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1023;

   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(outPin);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = (299*(signalMax - signalMin))/600;  // max - min = peak-peak amplitude
   //double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
   int numLedsToFill = (double)((signalMax)-320);
   if(numLedsToFill >300){numLedsToFill = 300;}
   if(numLedsToFill <0){numLedsToFill = 0;}
      unsigned int r = random(255);
   unsigned int g = random(255);
   unsigned int b = random(255);
  for(int i = 0;i<300;i++)
  {
    if(i<numLedsToFill){leds[i].red = r; leds[i].green=g; leds[i].blue=b;}
    else{leds[i]=CRGB::Black;}
//    leds[i] = CRGB::Red;
//    leds[i] = CHSV(peakToPeak,255,255);
   
  }
  FastLED.show();

  
   
   Serial.println(numLedsToFill);
}
