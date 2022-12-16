#include <ESP8266WiFi.h>

#include "Adafruit_MQTT.h"

#include "Adafruit_MQTT_Client.h"


const int moisturePin = A0;  // moisteure sensor pin

const int probe1 = 5; 

const int probe2 = 4; 

float moisturePercentage;              //moisture reading


const char *ssid =  "Galaxy-M20";     // Enter your WiFi Name

const char *pass =  "ac312124"; // Enter your WiFi Password


WiFiClient client;

#define MQTT_SERV "io.adafruit.com"

#define MQTT_PORT 1883

#define MQTT_NAME "aschoudhary"

#define MQTT_PASS "1ac95cb8580b4271bbb6d9f75d0668f1" 


Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

Adafruit_MQTT_Publish Moisture = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/Moisture");  // Moisture is the feed name where you will publish your data


const unsigned long Interval = 50000;

unsigned long previousTime = 0;


void setup()

{

  Serial.begin(9600);

  delay(10);

  pinMode(probe1, OUTPUT);

  pinMode(probe2, OUTPUT);

  digitalWrite(probe1, HIGH);

  digitalWrite(probe2, HIGH);

  Serial.println("Connecting to ");

  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)

  {

    delay(500);

    Serial.print(".");              // print ... till not connected

  }

  Serial.println("");

  Serial.println("WiFi connected");

   

}


void moisture()

{ 

  MQTT_connect();

  moisturePercentage = ( 100.00 - ( (analogRead(moisturePin) / 1023.00) * 100.00 ) ); 

  Serial.println("Soil Moisture is  = ");

  Serial.println(moisturePercentage);

  Serial.println("%");  

  delay(900); 

  Moisture.publish(moisturePercentage);

}


void loop()

{

  moisture();

  delay(3000);

  Serial.println("deep sleep for 1 hour");

  ESP.deepSleep(3600e6);

}



void MQTT_connect() 

{

  int8_t ret;

  // Stop if already connected.

  if (mqtt.connected()) 

  {

    return;

  }

  uint8_t retries = 3;

  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected

  { 

       mqtt.disconnect();

       delay(5000);  // wait 5 seconds

       retries--;

       if (retries == 0) 

       {

         // basically die and wait for WDT to reset me

         while (1);

       }

  }

}

