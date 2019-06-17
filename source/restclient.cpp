#include "restclient.h"

using namespace std;

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

RestClient::~RestClient()
{
}

void RestClient::Initialize(uint8_t *mac)
{
    Ethernet.begin(mac);
}

void RestClient::SaveGravity(int gravity)
{
}

int RestClient::Post(const char *path, const char *body)
{
    return Request("POST", path, body, NULL);
}

void RestClient::Write(const char *string)
{
    _client.print(string);
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

// The mother- generic request method.
//
int RestClient::Request(const char *method, const char *path, const char *body, String *response)
{

    if (_client.connect(_host, _port))
    {
        // Make a HTTP request line:
        Write(method);
        Write(" ");
        Write(path);
        Write(" HTTP/1.1\r\n");
        for (int i = 0; i < _num_headers; i++)
        {
            Write(_headers[i]);
            Write("\r\n");
        }
        Write("Host: ");
        Write(_host);
        Write("\r\n");
        Write("Connection: close\r\n");

        if (body != NULL)
        {
            char contentLength[30];
            Write(contentLength);

            Write("Content-Type: ");
            Write(_contentType);
            Write("\r\n");
        }

        Write("\r\n");

        if (body != NULL)
        {
            Write(body);
            Write("\r\n");
            Write("\r\n");
        }

        //make sure you Write all those bytes.
        delay(100);

        int statusCode = ReadResponse(response);

        //cleanup
        _num_headers = 0;
        _client.stop();
        delay(50);

        return statusCode;
    }
    else
    {
        return 0;
    }
}

int RestClient::ReadResponse(String *response)
{

    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    boolean httpBody = false;
    boolean inStatus = false;

    char statusCode[4];
    int i = 0;
    int code = 0;

    while (_client.connected())
    {

        if (_client.available())
        {
            char c = _client.read();

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
                //only Write response if its not null
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
    return code;
}