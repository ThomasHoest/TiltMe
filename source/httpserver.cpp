#include "httpserver.h"

using namespace std;

HttpServer::HttpServer()
{    
}

HttpServer::HttpServer(WiFiServer server)
{
    _server = &server;
}

HttpServer::~HttpServer()
{

}

void HttpServer::CreateResponse()
{
    WiFiClient client = _server->available();
  
  if (!client) 
  {
    return;
  }
  
  Serial.println("new client");
  
  while(!client.available())
  {
    delay(1);
  } 
  
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush(); 
  int value = LOW;
  
  if (request.indexOf("/LED=ON") != -1)  
  {
    digitalWrite(03, HIGH);
    value = HIGH;
  }
  
  if (request.indexOf("/LED=OFF") != -1)  
  {
    digitalWrite(03, LOW);
    value = LOW;
  }
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("");
  client.println("");
  client.print("Led is : "); 
  
  if(value == HIGH) 
  {
    client.print("On");
  } 
  else 
  {
    client.print("Off");
  }  
  
  client.println("");  
  client.println(" On ");
  client.println(" Off ");  
  client.println(" ");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");   
}