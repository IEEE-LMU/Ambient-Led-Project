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
   LEDS.addLeds<WS2812B,DATA_PIN,RGB>(leds,NUM_LEDS);
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
   peakToPeak = (signalMax - signalMin);  // max - min = peak-peak amplitude
   //double volts = (peakToPeak * 5.0) / 1024;  // convert to volts

  for(int i = 0;i<299;i++)
  {
    leds[i] = CHSV(peakToPeak,255,255);
   
  }
  FastLED.show();
  delay(100);
   
   Serial.println(peakToPeak);
}
