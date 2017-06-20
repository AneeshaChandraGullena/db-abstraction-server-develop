/* © Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM */

#ifndef __LUMBERJACK_PROTOSTUB
#define __LUMBERJACK_PROTOSTUB
extern "C" int LumberjackSendPairsAsMetrics( const char* ca_path, 
                const char* host, 
                const char* port,
                const char* spaceGUID,
                const char* token,
                const char* pairs );

extern "C" int LumberjackSendPairsAsLogs( const char* ca_path,
                const char* host,
                const char* port,
                const char* spaceGUID,
                const char* token,
                const char* pairs );

extern "C" void LumberjackTerm(int signum);

#endif //__NET_PORTABILITYSTUB
