#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 300 

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 12
#define CLOCK_PIN 10


const char *ssid = "LMU-Legacy";
const char *password = "IggyLion1";

const String endpoint = "http://api.openweathermap.org/data/2.5/weather?id=5370542&units=metric&APPID="; //ID for Marina Del Rey
const String key = "e15b344c704836a858ce0c1aa62db3b1";

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
	LEDS.setBrightness(84);

}

void loop() {

  /*if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status

    HTTPClient http;

    http.begin(endpoint + key); //Specify the URL
    int httpCode = http.GET();  //Make the request

    if (httpCode > 0) { //Check for the returning code

        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);

        StaticJsonBuffer<1000> jsonBuffer;

        JsonObject& root = jsonBuffer.parseObject(payload);   //Parse json respones

        if(root.success()) {
          Serial.println("Parsing success!");

        }else{
          Serial.println("Parsing error!");
        }

        double temp = root["main"]["temp"];
        Serial.print("Current temperature: ");
        Serial.println(temp);
      }

    else {
      Serial.println("Error on HTTP request");
    }

    http.end(); //Free the resources
  }

  delay(30000);*/

}