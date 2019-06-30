

//#include "libraries/httpserver/httpserver.h"
#include "wifimanager.h"
#include "restclient.h"
#include "httpserver.h"
#include "Scheduler.h"
#include "tilt.h"

const char *ssid = "**";             //your WiFi Name
const char *password = "**"; //Your Wifi Password
int ledPin = 03;

//WiFiServer server(80);
HttpServer *_httpServer;
WifiManager *_wifiManager;
Scheduler *_scheduler;
RestClient *_restClient;
Tilt *_tilt;

void testLoop()
{
    //_restClient->Post("/api/Gravity?value=32&sensorId=test2", "");

    
}

void ICACHE_RAM_ATTR interrupt(){
    _tilt->DmpDataReady();
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

    _tilt = new Tilt();
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), interrupt, RISING);
    //_tilt->Setup();
    
}

void loop()
{
    _scheduler->Tick();
    _httpServer->CreateResponse();    
}
