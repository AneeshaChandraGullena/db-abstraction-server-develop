/* © Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM */

#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <csignal>
#include "lumberjackstub.h"
#include "lumberjack.hpp"

#ifdef DEBUG
#define DERR(x) std::cerr << x
#define DOUT(x) std::cout << x
#else
#define DERR(x)
#define DOUT(x)
#endif

static ServiceManifest::MTLumberjackProtocol* lumberjack = NULL;

int LumberjackSendPairsAsMetrics( const char* ca_path, 
                const char* host, 
                const char* port,
                const char* spaceGUID,
                const char* token,
                const char* pairs)
{
    const char *str = "© Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM";
    signal(SIGTERM, LumberjackTerm);
    signal(SIGINT, LumberjackTerm);

    lumberjack = new ServiceManifest::MTLumberjackProtocol( std::string(ca_path),
            std::string(host),
            std::string(port),
            std::string(spaceGUID),
            std::string(token));

    DERR(" Metric Pairs: " << pairs << std::endl);

    if ( std::strlen(pairs) > 0 )
    {
        char* bufPairs = strdup(pairs);

        char* name = std::strtok(bufPairs, ",:");
        while ( name != NULL )
        {
            char* value = strtok( NULL, ",:" );
            //TODO: Improve Parsing
            if ( value == NULL )
            {
                lumberjack->addKeyValue(name,"1");
            }
            else
            {
                lumberjack->addKeyValue(name,value);
            }
            name = strtok( NULL, ",:");
        }
        if (bufPairs) 
           free(bufPairs);
    }

    int ret = lumberjack->sendMetrics();

   
    if (lumberjack)
        delete lumberjack;

    return ret; 
};

int LumberjackSendPairsAsLogs( const char* ca_path, 
                const char* host, 
                const char* port,
                const char* spaceGUID,
                const char* token,
                const char* pairs)
{
    signal(SIGTERM, LumberjackTerm);
    signal(SIGINT, LumberjackTerm);

    lumberjack = new ServiceManifest::MTLumberjackProtocol( std::string(ca_path),
        std::string(host),
        std::string(port),
        std::string(spaceGUID),
        std::string(token));

    DERR(" Log Pairs: " << pairs << std::endl);

    if ( std::strlen(pairs) > 0 )
    {
        char* bufPairs = strdup(pairs);

        char* name = std::strtok(bufPairs, ",:");
        while ( name != NULL )
        {
            //TODO: Improve terrible parsing logic :)
            char* value = strtok( NULL, ",:" );
            if ( value == NULL )
            {
                lumberjack->addKeyValue(name,"0");
            }
            else
            {
                lumberjack->addKeyValue(name, value);
            }

            name = strtok( NULL, ",:");
        }
  
        if (bufPairs)
            free(bufPairs);
    }

    int ret = lumberjack->sendLogs();

    if (lumberjack)
        delete lumberjack;

    return ret;    //CALLER MUST FREE recvBuffer
};

void LumberjackTerm(int signum)
{
    DERR("Received Signal...");
    if ( signum == SIGTERM || signum == SIGINT )
    {
        DERR("Received SIGTERM -> shutting down..");
        
        if ( lumberjack )
            delete lumberjack;

        exit(EXIT_FAILURE);
    }
};
