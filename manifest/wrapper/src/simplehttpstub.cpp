/* © Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM */

#include <iostream>
#include <string>
#include <cstring>
#include <csignal>
#include <stdlib.h>
#include "simplehttpstub.h"
#include "simplehttp.hpp"

#ifdef DEBUG
#define DERR(x) std::cerr << x
#define DOUT(x) std::cout << x
#else
#define DERR(x)
#define DOUT(x)
#endif

static ServiceManifest::SimpleHTTP* http = NULL;

int HTTPGet( const char* ca_path, 
                const char* host, 
                const char* port,
                const char* uri,
                const char* headers,
                const char* body,
                unsigned char*& response )
{
    const char *str = "© Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM";
    signal(SIGTERM, httpTerm);
    signal(SIGINT, httpTerm);

    http = new ServiceManifest::SimpleHTTP( std::string(ca_path),
            std::string(host),
            std::string(port));

    if ( body == NULL )
        body = "\0";

    if ( std::strlen(headers) > 0 )
    {
        char* bufHeaders = strdup(headers);

        char* name = std::strtok(bufHeaders, ",:");
        while ( name != NULL )
        {
            char* value = strtok( NULL, ",:" );
            
            //TODO Improve Parsing
            if ( value == NULL )
            {
                http->addHeader(name, "");
            }
            else
            {
                http->addHeader(name, value);
            }

            name = strtok( NULL, ",:");
        }
 
        if ( bufHeaders )
            free(bufHeaders);
    }

    int ret = http->GET(uri, body, response);

    //CALLER MUST FREE RESPONSE
    
    if ( http )
        delete http;

    return ret; 
}

int HTTPPut( const char* ca_path,
                const char* host,
                const char* port,
                const char* uri,
                const char* headers,
                const char* body,
                unsigned char*& response )
{
    signal(SIGTERM, httpTerm);
    signal(SIGINT, httpTerm);

    http = new ServiceManifest::SimpleHTTP( std::string(ca_path),
            std::string(host),
            std::string(port));

    if ( std::strlen(headers) > 0 )
    {
        char* bufHeaders = strdup(headers);

        char* name = std::strtok(bufHeaders, ",:");
        while ( name != NULL )
        {
            char* value = strtok( NULL, ",:" );
            
            //TODO Improve Parsing
            if ( value == NULL )
            {
                http->addHeader(name, "");
            }
            else
            {
                http->addHeader(name, value);
            }

            name = strtok( NULL, ",:");
        }
 
        if ( bufHeaders )
            free(bufHeaders);
    }

    int ret = http->PUT(uri, body, response);

    //CALLER MUST FREE RESPONSE

    if ( http )
        delete http;

    return ret; 
}

int HTTPPost( const char* ca_path,
                const char* host,
                const char* port,
                const char* uri,
                const char* headers,
                const char* body,
                unsigned char*& response )
{
    signal(SIGTERM, httpTerm);
    signal(SIGINT, httpTerm);

    http = new ServiceManifest::SimpleHTTP( std::string(ca_path),
            std::string(host),
            std::string(port));

    if ( std::strlen(headers) > 0 )
    {
        char* bufHeaders = strdup(headers);

        char* name = std::strtok(bufHeaders, ",:");
        while ( name != NULL )
        {
            char* value = strtok( NULL, ",:" );
            
            //TODO Improve Parsing
            if ( value == NULL )
            {
                http->addHeader(name, "");
            }
            else
            {
                http->addHeader(name, value);
            }

            name = strtok( NULL, ",:");
        }
 
        if ( bufHeaders )
            free(bufHeaders);
    }

    int ret = http->POST(uri, body, response);

    //CALLER MUST FREE RESPONSE

    if ( http )
        delete http;

    return ret; 
}

int HTTPDelete( const char* ca_path,
                const char* host,
                const char* port,
                const char* uri,
                const char* headers,
                const char* body,
                unsigned char*& response )
{
    signal(SIGTERM, httpTerm);
    signal(SIGINT, httpTerm);
    
    http = new ServiceManifest::SimpleHTTP( std::string(ca_path),
            std::string(host),
            std::string(port));

    if ( std::strlen(headers) > 0 )
    {
        char* bufHeaders = strdup(headers);

        char* name = std::strtok(bufHeaders, ",:");
        while ( name != NULL )
        {
            char* value = strtok( NULL, ",:" );
            
            //TODO Improve Parsing
            if ( value == NULL )
            {
                http->addHeader(name, "");
            }
            else
            {
                http->addHeader(name, value);
            }

            name = strtok( NULL, ",:");
        }

        if ( bufHeaders )
            free(bufHeaders);
    }

    int ret = http->PUT(uri, body, response);

     //CALLER MUST FREE RESPONSE   

    if ( http )
        delete http;

    return ret; 
}

int HTTPOptions( const char* ca_path,
                const char* host,
                const char* port,
                const char* uri,
                const char* headers,
                const char* body,
                unsigned char*& response )
{
    signal(SIGTERM, httpTerm);
    signal(SIGINT, httpTerm);
    
    http = new ServiceManifest::SimpleHTTP( std::string(ca_path),
            std::string(host),
            std::string(port));

    if ( std::strlen(headers) > 0 )
    {
        char* bufHeaders = strdup(headers);

        char* name = std::strtok(bufHeaders, ",:");
        while ( name != NULL )
        {
            char* value = strtok( NULL, ",:" );
            
            //TODO Improve Parsing
            if ( value == NULL )
            {
                http->addHeader(name, "");
            }
            else
            {
                http->addHeader(name, value);
            }

            name = strtok( NULL, ",:");
        }
        
        if ( bufHeaders )
           free(bufHeaders);
    }

    int ret = http->PUT(uri, body, response);

    //CALLER MUST FREE RESPONSE
    
    if ( http )
        delete http;

    return ret; 
}

void httpTerm ( int signal )
{
    DERR("Signal received. " << std::endl);

    if ( signal == SIGTERM || signal == SIGINT )
    {
        DERR("Shutting down.." << std::endl);
        if ( http )
            delete http;
    }
};
