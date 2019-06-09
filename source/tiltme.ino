#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureAxTLS.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>

//#include "libraries/httpserver/httpserver.h"
#include "wifimanager.h"
#include "httpserver.h"

const char *ssid = "supernet";             //your WiFi Name
const char *password = "thishouseisclean"; //Your Wifi Password
int ledPin = 03;

//WiFiServer server(80);
HttpServer *_httpServer;
WifiManager *_wifiManager;

void setup()
{
    Serial.begin(115200);
    delay(10);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    
    _wifiManager = new WifiManager((char*)ssid, (char*)password);
    _wifiManager->Connect();
    _wifiManager->StartServer();
    _httpServer = new HttpServer(_wifiManager);
}

void loop()
{
    _httpServer->CreateResponse();
}
