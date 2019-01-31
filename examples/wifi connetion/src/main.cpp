#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "LMU-Legacy";
const char *password = "IggyLion1";

void setup() {
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");

  //We can get the following info...
  IPAddress ip = WiFi.localIP();
  String mac = WiFi.macAddress();

  //And a bunch of other stuff. Check https://www.arduino.cc/en/Reference/WiFi for more!

  Serial.println(ip);
  Serial.println(mac);
}

void loop() {

}