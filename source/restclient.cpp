#include "restclient.h"

using namespace std;

#define HTTP_DEBUG

#ifdef HTTP_DEBUG
#define HTTP_DEBUG_PRINT(string) (Serial.print(string))
#endif

#ifndef HTTP_DEBUG
#define HTTP_DEBUG_PRINT(string)
#endif

RestClient::RestClient()
{
}

RestClient::RestClient(const char *host)
{
    _host = host;
    _port = 80;
    _num_headers = 0;
    _contentType = "application/x-www-form-urlencoded"; // default
}

RestClient::RestClient(const char *host, int port)
{
    _host = host;
    _port = port;
    _num_headers = 0;
    _contentType = "application/x-www-form-urlencoded"; // default
}

RestClient::RestClient(const char *host, int port, const char *fingerprint)
{
    _host = host;
    _port = port;
    _ssl = 1;
    _fingerprint = fingerprint;
    _num_headers = 0;
    if (!_contentType)
    {
        _contentType = "application/x-www-form-urlencoded"; // default
    }
}

RestClient::RestClient(const char *host, int port, int _ssl)
{
    _host = host;
    _port = port;
    _ssl = (_ssl) ? 1 : 0;
    _fingerprint = NULL;
    _num_headers = 0;
    if (!_contentType)
    {
        _contentType = "application/x-www-form-urlencoded"; // default
    }
}

bool RestClient::Dhcp()
{
    //84:0d:8e:8f:51:f9
  byte mac[] = { 0x84, 0x0D, 0x8E, 0x8F, 0x51, 0xF9 };
  if (begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    return false;
  }

  //give it time to initialize
  delay(1000);
  return true;
}

RestClient::~RestClient()
{
}

// GET path
int RestClient::Get(const char *path)
{
    return Request("GET", path, NULL, NULL);
}

//GET path with response
int RestClient::Get(const char *path, String *response)
{
    return Request("GET", path, NULL, response);
}

// POST path and body
int RestClient::Post(const char *path, const char *body)
{
    return Request("POST", path, body, NULL);
}

// POST path and body with response
int RestClient::Post(const char *path, const char *body, String *response)
{
    return Request("POST", path, body, response);
}

// PATCH path and body
int RestClient::Patch(const char *path, const char *body)
{
    return Request("PATCH", path, body, NULL);
}

// PATCH path and body with response
int RestClient::Patch(const char *path, const char *body, String *response)
{
    return Request("PATCH", path, body, response);
}

// PUT path and body
int RestClient::Put(const char *path, const char *body)
{
    return Request("PUT", path, body, NULL);
}

// PUT path and body with response
int RestClient::Put(const char *path, const char *body, String *response)
{
    return Request("PUT", path, body, response);
}

// DELETE path
int RestClient::Del(const char *path)
{
    return Request("DELETE", path, NULL, NULL);
}

// DELETE path and response
int RestClient::Del(const char *path, String *response)
{
    return Request("DELETE", path, NULL, response);
}

// DELETE path and body
int RestClient::Del(const char *path, const char *body)
{
    return Request("DELETE", path, body, NULL);
}

// DELETE path and body with response
int RestClient::Del(const char *path, const char *body, String *response)
{
    return Request("DELETE", path, body, response);
}

void RestClient::Write(const char *string)
{
    if (_ssl)
    {
        HTTP_DEBUG_PRINT("\nSSL Print: ");
        HTTP_DEBUG_PRINT(string);
        _sslClient.print(string);
    }
    else
    {
        HTTP_DEBUG_PRINT("\nHTTP Print: ");
        HTTP_DEBUG_PRINT(string);
        _client.print(string);
    }
}

void RestClient::SetHeader(const char *header)
{
    _headers[_num_headers] = header;
    _num_headers++;
}

void RestClient::SetContentType(const char *contentTypeValue)
{
    _contentType = contentTypeValue;
}

void RestClient::SetSSL(int ssl)
{
    _ssl = (ssl) ? 1 : 0;
}

// The mother- generic Request method.
//
int RestClient::Request(const char *method, const char *path,
                        const char *body, String *response)
{

    HTTP_DEBUG_PRINT("HTTP: connect\n");

    if (_ssl)
    {
        if (_fingerprint)
        {
            HTTP_DEBUG_PRINT("Verifiying SSL certificate\n");
            if (_sslClient.setFingerprint(_fingerprint))
            {
                HTTP_DEBUG_PRINT("SSL certificate matches\n");
            }
            else
            {
                HTTP_DEBUG_PRINT("SSL certificate does not match\n");
                return 0;
            }
        }

        if (!_sslClient.connect(_host, _port))
        {
            HTTP_DEBUG_PRINT("HTTPS Connection failed\n");
            return 0;
        }        
    }
    else
    {
        if (!_client.connect(_host, _port))
        {
            HTTP_DEBUG_PRINT("HTTP Connection failed\n");
            return 0;
        }
    }

    HTTP_DEBUG_PRINT("HTTP: connected\n");
    HTTP_DEBUG_PRINT("REQUEST: \n");

    String Request = String(method) + " " + String(path) + " HTTP/1.1\r\n";
    for (int i = 0; i < _num_headers; i++)
    {
        Request += String(_headers[i]) + "\r\n";
    }
    Request += "Host: " + String(_host) + ":" + String(_port) + "\r\n";
    Request += "Connection: close\r\n";
    if (body != NULL)
    {
        char contentLength[30];
        sprintf(contentLength, "Content-Length: %d\r\n", strlen(body));
        Request += String(contentLength);

        Request += "Content-Type: " + String(_contentType) + "\r\n";
    }
    Request += "\r\n";

    if (body != NULL)
    {
        Request += String(body);
        Request += "\r\n\r\n";
    }

    Write(Request.c_str());

    HTTP_DEBUG_PRINT("\nEND REQUEST\n");
    //make sure you write all those bytes.
    //delay(1000);

    HTTP_DEBUG_PRINT("HTTP: call readResponse\n");
    int statusCode = ReadResponse(response);
    HTTP_DEBUG_PRINT("HTTP: return readResponse\n");

    //cleanup
    HTTP_DEBUG_PRINT("HTTP: stop client\n");
    _num_headers = 0;
    if (_ssl)
    {
        _sslClient.stop();
    }
    else
    {
        _client.stop();
    }
    delay(50);
    HTTP_DEBUG_PRINT("HTTP: client stopped\n");

    return statusCode;
}

int RestClient::ReadResponse(String *response)
{

    // an http Request ends with a blank line
    boolean currentLineIsBlank = true;
    boolean httpBody = false;
    boolean inStatus = false;

    char statusCode[4];
    int i = 0;
    int code = 0;

    if (response == NULL)
    {
        HTTP_DEBUG_PRINT("HTTP: NULL RESPONSE POINTER: \n");
    }
    else
    {
        HTTP_DEBUG_PRINT("HTTP: NON-NULL RESPONSE POINTER: \n");
    }

    HTTP_DEBUG_PRINT("HTTP: RESPONSE: \n");
    void *http_client;
    if (_ssl)
    {
        HTTP_DEBUG_PRINT("HTTP: Connect: " + String(_sslClient.connected()) + " Available: " + String(_sslClient.available()) + "\n");
        while (_sslClient.connected())
        {
            HTTP_DEBUG_PRINT(".");

            if (_sslClient.available())
            {
                HTTP_DEBUG_PRINT(",");

                char c = _sslClient.read();
                HTTP_DEBUG_PRINT(c);

                if (c == ' ' && !inStatus)
                {
                    inStatus = true;
                }

                if (inStatus && i < 3 && c != ' ')
                {
                    statusCode[i] = c;
                    i++;
                }
                if (i == 3)
                {
                    statusCode[i] = '\0';
                    code = atoi(statusCode);
                }

                if (httpBody)
                {
                    //only write response if its not null
                    if (response != NULL)
                        response->concat(c);
                }
                else
                {
                    if (c == '\n' && currentLineIsBlank)
                    {
                        httpBody = true;
                    }

                    if (c == '\n')
                    {
                        // you're starting a new line
                        currentLineIsBlank = true;
                    }
                    else if (c != '\r')
                    {
                        // you've gotten a character on the current line
                        currentLineIsBlank = false;
                    }
                }
            }
        }
        HTTP_DEBUG_PRINT("HTTPS _client closed \n");
    }
    else
    {
        while (_client.connected())
        {
            HTTP_DEBUG_PRINT(".");

            if (_client.available())
            {
                HTTP_DEBUG_PRINT(",");

                char c = _client.read();
                HTTP_DEBUG_PRINT(c);

                if (c == ' ' && !inStatus)
                {
                    inStatus = true;
                }

                if (inStatus && i < 3 && c != ' ')
                {
                    statusCode[i] = c;
                    i++;
                }
                if (i == 3)
                {
                    statusCode[i] = '\0';
                    code = atoi(statusCode);
                }

                if (httpBody)
                {
                    //only write response if its not null
                    if (response != NULL)
                        response->concat(c);
                }
                else
                {
                    if (c == '\n' && currentLineIsBlank)
                    {
                        httpBody = true;
                    }

                    if (c == '\n')
                    {
                        // you're starting a new line
                        currentLineIsBlank = true;
                    }
                    else if (c != '\r')
                    {
                        // you've gotten a character on the current line
                        currentLineIsBlank = false;
                    }
                }
            }
        }
    }

    HTTP_DEBUG_PRINT("HTTP: return readResponse3\n");
    return code;
}