/* © Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM */

#ifndef __SIMPLEHTTP_PROTOSTUB
#define __SIMPLEHTTP_PROTOSTUB
extern "C" int HTTPGet( const char* ca_path, 
                const char* host, 
                const char* port,
                const char* uri,
                const char* headers,
                const char* body,
                unsigned char*& response );

extern "C" int HTTPPut( const char* ca_path,
                const char* host,
                const char* port,
                const char* uri,
                const char* headers,
                const char* body,
                unsigned char*& response );

extern "C" int HTTPPost( const char* ca_path,
                const char* host,
                const char* port,
                const char* uri,
                const char* headers,
                const char* body,
                unsigned char*& response );

extern "C" int HTTPDelete( const char* ca_path,
                const char* host,
                const char* port,
                const char* uri,
                const char* headers,
                const char* body,
                unsigned char*& response );

extern "C" int HTTPOptions( const char* ca_path,
                const char* host,
                const char* port,
                const char* uri,
                const char* headers,
                const char* body,
                unsigned char*& response );

extern "C" void httpTerm( int signal );
#endif //__NET_PORTABILITYSTUB
