/* IEEE AMBIRNET LIGHT PROJECT */
// begin with necessary includes 
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>

// defines to pins go here, randomized for now.
#define to_led   1
#define two   2
#define three 3
#define lim   100

// network name and password
const char *ssid = "LMU-Legacy";
const char *pass = "IggyLion1";

// initialize needed vars
    char input[lim];
    int  dex = 0;
    int  num = 0;

// initiate Wifi Server (I think...)
WiFiServer srv(80);

void setup(){
    // set up ESP32
    Serial.begin(115200);
    pinMode(to_led, OUTPUT);

    // establish connection to server

    // attempt to connect to given server
    Serial.print("Connecting to ID: ");
    Serial.println(ssid);

    WiFi.begin(ssid, pass);

    while(WiFi.status() != WL_CONNECTED){
        delay(1000);
        Serial.print("a");
    }

    Serial.println("nndd..\nSuccess!");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    
    srv.begin();

}

void loop(){
    WiFiClient client = srv.available();
    if(client){
        Serial.println("Client #" + String(num) +  "has connected.");
        num++;
        
        // reset the index for every new connection
        dex = 0;

        while(client.connected()){
            if(client.available()){
                // begin reading input from 
                char ch = client.read();

                // add character to the array if input is not 'enter'
                if(ch != '\n' && ch != '\r'){
                    input[dex] = ch;
                    dex++;
                }

                // necessary to read more data from user
                continue;
            }
            // send an http response header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");

            // LMU Logo
            client.println("<img src=\"https://ibb.co/VMH3c7h\"/>\ ");
            client.println();

            break;

        }
    }
}