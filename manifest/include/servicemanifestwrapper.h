/* © Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM */

#ifndef __DL_WRAPPER__
#define __DL_WRAPPER__
extern      const char*  getCatalogServiceName( const char* libpath = "libibmmanifest.so.1"  );
extern      const char*  getComponentName(const char* libpath = "libibmmanifest.so.1" );
extern      const char*  getResourceType( const char* libpath = "libibmmanifest.so.1" );
extern      const char*  getResourceName( const char* libpath = "libibmmanifest.so.1" );
extern      const char*  getPagerDutyTeamURL( const char* libpath = "libibmmanifest.so.1" );
extern      const char*  getBaileyProjectName( const char* libpath = "libibmmanifest.so.1" );
extern      const char*  getBaileyInstanceURL( const char* libpath = "libibmmanifest.so.1" );
extern      const char*  getTenancyModel( const char* libpath = "libibmmanifest.so.1" );
extern      const char*  getTeamEmailAddress( const char* libpath  = "libibmmanifest.so.1" );
extern      const char*  getInstanceId( const char* libpath = "libibmmanifest.so.1" );
extern      const char*  getSourceCodeRepoURL( const char* libpath = "libibmmanifest.so.1" );
extern      const char*  getCRN( const char* libpath = "libibmmanifest.so.1", const char delimit = ':' );
extern      const char*  getFQComponentName ( const char* libpath = "libibmmanifest.so.1", const char delimit = ':' );
extern      const char*  getManifestJSON( const char* libpath = "libibmmanifest.so.1" );

extern      int     LumberjackSendPairsAsMetrics( 
                        const char* host,
                        const char* port,
                        const char* spaceGUID,
                        const char* token,
                        const char* sendPairs,
                        const char* ca_path = "/etc/ssl/certs",
                        const char* libpath = "libibmmanifest.so.1");

extern      int     LumberjackSendPairsAsLogs( 
                        const char* host,
                        const char* port,
                        const char* spaceGUID,
                        const char* token,
                        const char* sendPairs,
                        const char* ca_path = "/etc/ssl/certs",
                        const char* libpath = "libibmmanifest.so.1");

extern      int     HTTPGet ( 
                        const char* host,
                        const char* port,
                        const char* uri,
                        const char* headers,
                        const char* body,
                        unsigned char** response,
                        const char* ca_path = "/etc/ssl/certs",
                        const char* libpath = "libibmmanifest.so.1");

extern      int     HTTPPost ( 
                        const char* host,
                        const char* port,
                        const char* uri,
                        const char* headers,
                        const char* body,
                        unsigned char** response,
                        const char* ca_path = "/etc/ssl/certs",
                        const char* libpath= "libibmmanifest.so.1" );

extern      int     HTTPPut ( 
                        const char* host,
                        const char* port,
                        const char* uri,
                        const char* headers,
                        const char* body,
                        unsigned char** response,
                        const char* ca_path = "/etc/ssl/certs",
                        const char* libpath = "libibmmanifest.so.1" );

extern      int     HTTPDelete ( 
                        const char* host,
                        const char* port,
                        const char* uri,
                        const char* headers,
                        const char* body,
                        unsigned char** response,
                        const char* ca_path = "/etc/ssl/certs",
                        const char* libpath = "libibmmanifest.so.1" );

extern      int     HTTPOptions ( 
                        const char* host,
                        const char* port,
                        const char* uri,
                        const char* headers,
                        const char* body,
                        unsigned char** response,
                        const char* ca_path = "/etc/ssl/certs",
                        const char* libpath = "libibmmanifest.so.1" );
#endif//__DL_WRAPPER__
