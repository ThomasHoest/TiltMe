#include "httpserver.h"

using namespace std;

HttpServer::HttpServer()
{
}

HttpServer::HttpServer(WifiManager *manager)
{
    _manager = manager;
}

HttpServer::~HttpServer()
{
}

void HttpServer::CreateResponse()
{
    WiFiClient client = _manager->GetServer()->available();

    if (!client)
    {
        return;
    }

    Serial.println("new client");

    while (!client.available())
    {
        delay(1);
    }

    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("");
    client.println("");
    client.print("Gravity: ");
    delay(1);
    Serial.println("Client disonnected");
    Serial.println("");    
}