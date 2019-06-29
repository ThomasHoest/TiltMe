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
#include "restclient.h"
#include "httpserver.h"
#include "Scheduler.h"

const char *ssid = "supernet";             //your WiFi Name
const char *password = "thishouseisclean"; //Your Wifi Password
int ledPin = 03;

//WiFiServer server(80);
HttpServer *_httpServer;
WifiManager *_wifiManager;
Scheduler *_scheduler;
RestClient *_restClient;

void testLoop()
{
    _restClient->Post("/api/Gravity?value=32&sensorId=test2", "");

    // String response = "";
    // _restClient->Get("/api/Sensor/sensors", &response);

    // String response = "";
    // int statusCode = _restClient->Get("/get", &response);
    // Serial.print("Status code from server: ");
    // Serial.println(statusCode);
    // Serial.print("Response body from server: ");
    // Serial.println(response);
}

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
    _restClient = new RestClient("tiltmeservice.azurewebsites.net", 443, "bb e8 5f 73 06 56 b2 97 8b ac 2e 98 49 76 00 e6 d4 51 28 bd");
    _restClient->Dhcp();    
    _restClient->SetContentType("application/json");

    _scheduler = new Scheduler();    
    _scheduler->ScheduleTask(testLoop, 10000);
    
}

void loop()
{
    _scheduler->Tick();
    _httpServer->CreateResponse();    
}
