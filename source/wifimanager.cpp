#include "wifimanager.h"

using namespace std;

WifiManager::WifiManager()
{
}

WifiManager::WifiManager(char *ssid, char *password)
{
    _ssid = ssid;
    _password = password;    
}

WifiManager::~WifiManager()
{
}

WiFiServer* WifiManager::GetServer()
{
    return _server;
}

void WifiManager::Connect()
{
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(_ssid);
    WiFi.begin(_ssid, _password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");    
    Serial.println("Server started");
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    
}

uint8_t *WifiManager::GetMacAddress()
{
    uint8_t mac[6];
    return WiFi.macAddress(mac);
}

void WifiManager::StartServer()
{
    _server = new WiFiServer(80);
    _server->begin();    
}
