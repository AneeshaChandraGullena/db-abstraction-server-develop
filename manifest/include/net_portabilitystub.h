/* © Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM */

#ifndef __NET_PORTABILITYSTUB
#define __NET_PORTABILITYSTUB

extern "C" int TLSsendReceiveStream( const char* ca_path, 
                const char* host, 
                const char* port, 
                const unsigned char* sendBuffer, 
                unsigned char*& recvBuffer,
                size_t& recvBufferSz);

#endif //__NET_PORTABILITYSTUB
