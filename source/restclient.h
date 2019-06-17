#ifndef restclient_h
#define restclient_h

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include "wifimanager.h"

class RestClient
{
public:
    RestClient();
    RestClient(const char *host);
    RestClient(const char *host, int port);

    ~RestClient();
    void Initialize(uint8_t *mac);
    void SaveGravity(int gravity);
    int Post(const char *path, const char *body);

private:
    void Write(const char *string);
    void SetHeader(const char *header);
    void SetContentType(const char *contentTypeValue);

    int ReadResponse(String *response);
    int Request(const char *method, const char *path, const char *body, String *response);

    const char *_host;
    EthernetClient _client;
    int _port;
    int _num_headers;
    const char *_headers[10];
    const char *_contentType;
};

#endif