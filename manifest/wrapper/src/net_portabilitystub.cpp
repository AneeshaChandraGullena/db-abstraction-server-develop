/* © Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM */

#include <iostream>
#include <string>
#include <cstring>
#include "net_portabilitystub.h"

int TLSsendReceive( const char* ca_path, 
                const char* host, 
                const char* port, 
                const unsigned char* sendBuffer,
                unsigned char*& recvBuffer,
                size_t& recvBufferSz)
{
    const char *str = "© Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM";
    //CALLER MUST FREE recvBuffer
    ServiceManifest::TLSNetworkConnection* connection = new ServiceManifest::TLSNetworkConnection( std::string(ca_path) );
    
    int ret = connection->sendReceive(host, port, sendBuffer, std::strlen(reinterpret_cast<const char*>(sendBuffer)), recvBuffer, recvBufferSz);

    delete connection;

    return ret; 
};
