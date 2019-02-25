#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <time.h>
#include <FastLED.h>

#define SUN_TIME 3600.0f*2.0f
#define NUM_LEDS 300 
#define DATA_PIN 12

struct Weather {
  float currTemp;
  float minTemp;
  float maxTemp;
  unsigned long sunrise;
  unsigned long sunset;
};

enum Mode {
  TEMP,
  SUN,
  EFFECTS,
  OFF
};

bool modeChanged = true;

const char *ssid = "LMU-Legacy";
const char *password = "IggyLion1";

const String endpoint = "http://api.openweathermap.org/data/2.5/weather?id=5370542&units=metric&APPID="; //ID for Marina Del Rey
const String key = "e15b344c704836a858ce0c1aa62db3b1";

const char* ntpServer = "pool.ntp.org";

WebServer server(80);

CRGB daylight = {198, 163, 255};
CRGB sunset = {146, 255, 50};

//GREEN, RED, BLUE


CRGB leds[NUM_LEDS];

Weather weather;

Mode mode;

unsigned long lastWeatherPoll = 0;
unsigned long lastLightUpdate = 0;

int color = 0;

void wifiConnect(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupTime(){
  configTime(0,0,ntpServer);
  /*struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");*/
  delay(1000);
  Serial.print("Got current time! ");
  Serial.println(time(NULL));
}

void pollWeatherService(){
  HTTPClient http;
  http.begin(endpoint + key); //Specify the URL
  int httpCode = http.GET();  //Make the request

  if (httpCode > 0) { //Check for the returning code

      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);

      StaticJsonBuffer<2000> jsonBuffer;

      JsonObject& root = jsonBuffer.parseObject(payload);   //Parse json respones

      if(root.success()) {
        Serial.println("Parsing success!");
      }else{
        Serial.println("Parsing error!");
      }

      weather.currTemp = root["main"]["temp"];
      weather.minTemp = root["main"]["temp_min"];
      weather.maxTemp = root["main"]["temp_max"];
      weather.sunrise = root["sys"]["sunrise"];
      weather.sunset = root["sys"]["sunset"];

      unsigned long lastTime = time(NULL);
      unsigned long oneDay = 3600*24;
      if (weather.sunset - lastTime > oneDay/2){ //If the sunset is more than 12 hours away...
        weather.sunset -= oneDay;                //Bring both timestamps closer
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

void sendToAllLeds(CRGB value){
  for(int j = 0; j < NUM_LEDS; j++) {
    leds[j] = value;
  }
  FastLED.show();
}

void handleRoot(){
  char html [400];
  snprintf(html, 400,
   "<html>\nClick <a href=\"/TEMP\">here</a> for temperature mode.<br>\nClick <a href=\"/SUN\">here</a> for sunset mode.<br>\nClick <a href=\"/EFFECTS\">here</a> for effects mode<br>\nClick <a href=\"/OFF\">here</a> to turn LEDs off<br></html>"
  );
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  wifiConnect();
  delay(1000);
  setupTime();
  pollWeatherService();

  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
	LEDS.setBrightness(255);

  server.on("/", handleRoot);     //Define functions to be called when 
  server.on("/TEMP",[]() {
    mode = TEMP;
  });
  server.on("/SUN",[]() {
    mode = SUN;
  });
  server.on("/EFFECTS",[]() {
    mode = EFFECTS;
  });
  server.on("/OFF",[]() {
    mode = OFF;
    modeChanged = true;
  });
  server.begin();

  mode = EFFECTS;

  
}

void loop() {
  switch (mode){
    case SUN:
      if (millis() - lastWeatherPoll > 30000){   //Poll weather service every 30 seconds
        pollWeatherService();
        lastWeatherPoll = millis();
      }

      if (millis() - lastLightUpdate > 1000){
        unsigned long currentTime = time(NULL);
        float dimming;
        long distToSunset = weather.sunset - currentTime;
        long distToSunrise = weather.sunrise - currentTime;
        
        if (distToSunset < distToSunrise){
          dimming = (distToSunset/(SUN_TIME) + 1.0)/2;  //1 if before sunset, 0 after sunset
        }else{
          dimming = (-distToSunrise/(SUN_TIME) + 1.0)/2;  //0 if before sunrise, 1 after sunrise
        }

        Serial.println(dimming);

        if (dimming <= 0.0f){
          dimming = 0.0f;
        }else if (dimming >= 1.0f){
          dimming = 1.0f;
        }

        dimming = 0;

        Serial.print("Scaling: ");
        Serial.println(dimming);

        CRGB value;
        value.r = sunset.r*(1.0f-dimming) + daylight.r*dimming;
        value.g = sunset.g*(1.0f-dimming) + daylight.g*dimming;
        value.b = sunset.b*(1.0f-dimming) + daylight.b*dimming;

        sendToAllLeds(value);

        lastLightUpdate = millis();
      }
    break;

    case TEMP:
    
    break;

    case EFFECTS:
      if (millis() - lastLightUpdate > 10){
        sendToAllLeds(CHSV(color,255,255));
        color++;
        color = color%255;
        lastLightUpdate = millis();
      }
    break;

    case OFF:
      if (modeChanged){
        sendToAllLeds({0,0,0});
      }else{
        modeChanged = false;
      }
    break;
  }
  server.handleClient();
}