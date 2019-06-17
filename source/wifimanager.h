#ifndef wifimanager_h
#define wifimanager_h

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

class WifiManager
{
    public:    
    WifiManager();
    WifiManager(char *ssid, char *password);
    ~WifiManager();    

    void Connect();
    void StartServer();
    WiFiServer *GetServer();
    uint8_t *GetMacAddress();
    
    private:
    char *_ssid;
    char *_password;
    WiFiServer *_server;
};

#endif