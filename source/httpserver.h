#ifndef httpserver_h
#define httpserver_h

#include "wifimanager.h"

class HttpServer
{
    public:
    
    HttpServer();
    HttpServer(WifiManager *manager);
    ~HttpServer();    
    void CreateResponse();    

    private:
    WifiManager *_manager;
};

#endif