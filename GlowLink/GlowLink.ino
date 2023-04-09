#include <WiFiManager.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <FastLED.h>

#define NUM_LEDS 51
#define DATA_PIN 4

WiFiManager wifiManager;
WiFiServer server(80);
WiFiClient client;
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  
  // try to connect to WiFi network
  if (!wifiManager.autoConnect("Blaze")) {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  IPAddress gateway = WiFi.gatewayIP();
  
  IPAddress ip = IPAddress(gateway[0], gateway[1], gateway[2], 172);
  IPAddress subnet = IPAddress(255,255,255,0);
  WiFi.config(ip, gateway, subnet);

    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
}

int x = 0;

byte rgb[] = {255,255,255};


void loop()
{
  ArduinoOTA.handle();

 
byte i=0;
  rgb[0] = 0;
  rgb[1] = 0;
  rgb[2] = 0;
  if (client.connected()) {
    if (client.available()) {
      while (client.available()) {
        char x = client.read();
        if(x==' ')
        i++;
        else
        rgb[i]= rgb[i]*10+(x-'0');
      }
      fill_solid(leds, NUM_LEDS, CRGB(rgb[0], rgb[1], rgb[2]));
      FastLED.show(); 
    }
  } else {
    client = server.available();
  }
      
}











/*
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <FastLED.h>

#define NUM_LEDS 60
#define DATA_PIN 4

WiFiServer server(80);
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  
  WiFi.begin("Dropkick", "avijitdasxp");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  server.begin();

   FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
   FastLED.setBrightness(255);
}

WiFiClient client;
byte rgb[] = {0,0,0};

void loop() {

  byte i=0;
  rgb[0] = 0;
  rgb[1] = 0;
  rgb[2] = 0;
  if (client.connected()) {
    if (client.available()) {
      while (client.available()) {
        char x = client.read();
        if(x==' ')
        i++;
        else
        rgb[i]= rgb[i]*10+(x-'0');
      }
      Serial.printf("%d %d %d\n", rgb[0], rgb[1], rgb[2]);
      fill_solid(leds, NUM_LEDS, CRGB(rgb[0], rgb[1], rgb[2]));
      FastLED.show(); 
    }
  } else {
    client = server.available();
  }
}

*/
