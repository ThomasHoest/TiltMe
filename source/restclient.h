#ifndef restclient_h
#define restclient_h

//#include <Arduino.h>
#include <Ethernet.h>
#include <ESP8266WiFi.h>
//#include <WiFi101.h>

class RestClient
{
public:
    RestClient();
    RestClient(const char *host);
    RestClient(const char *host, int port);
     // set ssl to on but do not verify server identity with fingerprint
    RestClient(const char *host, int port, int ssl);
    // set fingerprint if using SSL, stores the SHA1 fingerprint of the remote site, implicity sets ssl to on
    RestClient(const char *host, int port, const char* fingerprint);

    ~RestClient();
    //void Initialize(uint8_t *mac);
    //void SaveGravity(int gravity);
    
     //Client Setup
    bool Dhcp();
    
    //Generic HTTP Request
    int Request(const char* method, const char* path,
                const char* body, String* response);
    // Set a Request Header
    void SetHeader(const char*);
    // Set Content-Type Header
    void SetContentType(const char*);
    // Set SSL support on(1) or off(0)
    void SetSSL(int);

    // GET path
    int Get(const char*);
    // GET path and response
    int Get(const char*, String*);

    // POST path and body
    int Post(const char* path, const char* body);
    // POST path and body and response
    int Post(const char* path, const char* body, String*);

    // PATCH path and body
    int Patch(const char* path, const char* body);
    // PATCH path and body and response
    int Patch(const char* path, const char* body, String*);

    // PUT path and body
    int Put(const char* path, const char* body);
    // PUT path and body and response
    int Put(const char* path, const char* body, String*);

    // DELETE path
    int Del(const char*);
    // DELETE path and body
    int Del(const char*, const char*);
    // DELETE path and response
    int Del(const char*, String*);
    // DELETE path and body and response
    int Del(const char*, const char*, String*);


private:

    int ReadResponse(String*);
    void Write(const char*);

    const char *_host;
    WiFiClient _client;
    WiFiClientSecure _sslClient;
    int _port;
    int _ssl;
    int _num_headers;
    const char *_headers[10];
    const char *_contentType;
    const char *_fingerprint;
    const char *_sensorId;
};

#endif