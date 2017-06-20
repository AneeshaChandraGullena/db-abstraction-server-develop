/* © Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "ceg_deploy.h"
#include "servicemanifestwrapper.h"

#ifdef DEBUG
#define DERR(fmt, args...) fprintf(stderr, fmt, ## args )
#define DOUT(fmt, args...) fprintf(stdout, fmt, ## args )
#else
#define DERR(fmt, args...)
#define DOUT(fmt, args...)
#endif

const  char* builtManifest = "BuiltManifest";

static char* opName;
static char* soPath;
static char* certPath;
static char* hostName;
static char* port;
static char* token;
static char* spaceGUID;
static char* registryURI;

const char* copySourceAndReplaceCommasAndColons( const char* source )
{
    char* result = NULL;

    if ( source )
    {
        result = (char*)calloc(strlen(source), sizeof(char));
        char* d = result;
        for ( const char* p = source; *p; p++ )
        {
            if ( *p == ',' )
            {
                *d = '~';
                d++;
                continue;
            }

            if ( *p == ':' )
            {
                *d = ';';
                d++;
                continue;
            }

            if ( *p == ' ' )
            {
                *d = '_';
                d++;
                continue;
            }

            *d = *p;
            d++;
        };
    }

    return (const char*) result;
}

const char* urlEncode( const char* source )
{
    //TODO: Implement URL Escaping correclty
    static char hexVal[] = "0123456789ABCDEF";

    const char* p = source;
    int extraSpace = 0;

    while ( *p )
    {
        if ( *p != isalnum(*p)
            && *p != '_'
            && *p != '-'
            && *p != '.'
            && *p != ';'
            && *p != '\''
            && *p != '('
            && *p != ')'
            && *p != '!' )
            extraSpace = extraSpace + 3;

        p++;
    }

    //Caller must free escapped string
    char* result = (char*)calloc(strlen(source) + extraSpace, sizeof(char));

    p = source;
    char* d = result;
    while ( *p )
    {
        if ( isalnum(*p) == 0 &&
            *p != '_' &&
            *p != '-' &&
            *p != '.' &&
            *p != ';' &&
            *p != '\'' &&
            *p != '(' &&
            *p != ')' &&
            *p != '!' )
        {
            *d++ = '%';
            *d++ = hexVal[ (*p >> 4) & 15];
            *d++ = hexVal[ (*p) & 15 ];
        }
        else
        {
            *d++ = *p;
        }

        p++;
    }

    return (const char*) result;
}

int parseArgsForLM ( int argc, char** argv )
{
    if ( argc < 8 )
    {
        DOUT("Usage:\r\n");
        DOUT("%s <opname> <sopath> <certificate path> <host> <port> <access token> <space guid> \r\n", argv[0]);
        return -1;
    }

    opName = argv[1];
    soPath = argv[2];
    certPath = argv[3];
    hostName = argv[4];
    port = argv[5];
    token = argv[6];
    spaceGUID = argv[7];

    return 0;
}

int parseArgsForRegistry ( int argc, char** argv )
{
    if ( argc < 7 )
    {
        DOUT("Usage:\r\n");
        DOUT("%s <opname> <sopath> <certificate path> <host> <port> <uri> <optional - access token> \r\n", argv[0]);
        return - 1;
    }

    opName = argv[1];
    soPath = argv[2];
    certPath = argv[3];
    hostName = argv[4];
    port = argv[5];
    registryURI = argv[6];

    if ( argc == 8 )
        token = argv[7];

    return 0;
}

int main( int argc, char** argv )
{
    const char *copyright_str = "© Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM";

    printf("Service Manifest Registration & LOGMET Tool v1.0\n");
    printf("%s\n", copyright_str);

    //TODO: Fix to find multicall verb properly
    //strstr use is a hack - has side effects, can't have folders
    //paths, or parts of the multicall verbs named the same as
    //any other multicall verbs

    //POSIX - argv[0] is always the stem of the command issued
    //from the command line that invoked the program
    //-- usually this is the same as the file name
    // but the multicall binary
    int ret = 0;

    if ( strstr(argv[0], Instrument) != NULL )
    {
        if ( parseArgsForLM (argc, argv) < 0 )
            return 1;

        ret = recordBuildWithMetric(builtManifest, "1");
    }
    else if ( strstr( argv[0], Log) != NULL )
    {
        if ( parseArgsForLM (argc, argv) < 0 )
            return 1;

        if ( argc == 8 )
        {
            ret = recordBuildWithLog(argv[1], argv[1]);
        }
        else if ( argc == 9 )
        {
            ret = recordBuildWithLog(argv[1], argv[8]);
        }
    }
    else if ( strstr( argv[0], Register ) != NULL )
    {
        if ( parseArgsForRegistry ( argc, argv ) < 0 )
            return 1;

        if ( strstr(opName,cegRegisterGet) != NULL )
        {
            ret = getManifestFromRegistry();
        }
        else if ( strstr(opName, cegRegisterPost) != NULL )
        {
            ret = postManifestToRegistry();
        }
        else if ( strstr(opName, cegRegisterPut) != NULL )
        {
            ret = putManifestToRegistry();
       }
    }

    DERR("ceg_deploy returned %d\r\n ", ret);
    fprintf(stdout, "RET is %d\n", ret);
    if (ret == 0 || (ret >= 200 && ret <= 299))
    {
      printf("Registration & LOGMET tool: SUCCESS\n");
    }
    else
    {
      printf("Registration & LOGMET tool: FAILED\n");
    }
    return ret;
};

int getManifestFromRegistry()
{
    const char* catalogName = getCatalogServiceName(soPath);
    const char* componentName = getComponentName(soPath);

    const char* encodedName = urlEncode(catalogName);
    const char* encodedComponentName = urlEncode(componentName);

    const char* uriFormat = "%s?q=servicename:%s+componentname:%s";
    unsigned char* response = NULL;

    if (catalogName)
        free((char*)catalogName);

    if ( componentName )
        free((char*)componentName);

    size_t len = strlen(registryURI) + strlen(encodedName) + strlen(encodedComponentName) + 30;
    char* uri = (char*)calloc(len, sizeof(char));

    snprintf(uri, len, uriFormat, registryURI, encodedName, encodedComponentName);

    DERR("cegRegister - GET: %s \r\n ", uri);

    int ret = HTTPGet(hostName, port, uri, "", "", &response, certPath, soPath);

    DERR("cegRegister - GET Response: %s \r\n", response);

    if ( uri )
        free((char*)uri);

    if ( encodedName )
        free((char*)encodedName);

    if ( encodedComponentName )
        free((char*)encodedComponentName);

    if ( response )
        free((char*)response);

    return ret;
};

int putManifestToRegistry()
{
    DERR("cegRegister - using SO: %s \r\n", soPath);

    const char* manifestJSON = getManifestJSON(soPath);

    unsigned char* response = NULL;

    DERR("cegRegister - PUT uri: %s \r\n",  registryURI );
    DERR("cegRegister - PUT body: %s \r\n", manifestJSON );

    int ret = HTTPPut( hostName, port, registryURI, "content-type:application/json", manifestJSON, &response, certPath, soPath);
    DERR("cegRegister - PUT reponse code: %d \r\n", ret);

    if (ret < 200 || ret > 299)
      fprintf(stderr, "%s: Server replied [%s]\n", Register, response);

    if ( manifestJSON )
        free((char*)manifestJSON);

    if ( response )
        free((char*)response);

    return ret;
};

int postManifestToRegistry()
{
    DERR("cegRegister - using SO: %s \r\n", soPath);

    const char* manifestJSON = getManifestJSON(soPath);

    unsigned char* response = NULL;

    DERR("cegRegister - POST uri: %s \r\n",  registryURI );
    DERR("cegRegister - POST body: %s \r\n", manifestJSON );

    int ret = HTTPPost( hostName, port, registryURI, "content-type:application/json", manifestJSON, &response, certPath, soPath);
    DERR("cegRegister - POST Response code: %d \r\n", ret);

    if (ret < 200 || ret > 299)
      fprintf(stderr, "%s: Server replied [%s]\n", Register, response);

    if ( manifestJSON )
        free((char*)manifestJSON);

    if ( response )
        free((char*)response);

    return ret;
};

int recordBuildWithMetric(const char* name, const char* value)
{
    int ret = 0;

    //HACK - scrub commas and colons - for LogMet and our
    // weak parsing of the Pairs String
    // makes a copy of crn - so both must be freed
    //
    const char* raw = getFQComponentName(soPath, '.');
    const char* fqname = copySourceAndReplaceCommasAndColons(raw);

    if (raw)
        free((char*)raw);

    char* pairs;

    DERR("\r\n Got FQName: %s \r\n", fqname);

    if ( fqname )
    {
        size_t len = (strlen(fqname) + strlen(name) + strlen(value) + 3) * sizeof(char);

        char* pairs = (char*)calloc(len, sizeof(char));
        snprintf(pairs, len, "%s.%s:%s", fqname, name, value);

        DERR("Metric: %s \r\n", pairs);

        ret = LumberjackSendPairsAsMetrics( hostName, port, spaceGUID, token, pairs, certPath, soPath);

        DERR("SendPairsAsMetrics returned: %d\r\n", ret);

        if ( pairs )
            free((char*)pairs);
   }

    if ( fqname )
        free((char*)fqname);

    return ret;
};

int recordBuildWithLog( const char* name, const char* log )
{
    int ret = 0;

    const char* raw = getFQComponentName(soPath, '.');
    const char* fqname = copySourceAndReplaceCommasAndColons(raw);

    if (raw)
        free((char*)raw);

    DERR("\r\n Got FQName: %s \r\n ", fqname);

    if ( fqname )
    {
        size_t len = (strlen(fqname) + strlen(name) + strlen(log) + 3) * sizeof(char);
        char* logs = (char*)calloc(len,sizeof(char));
        snprintf(logs, len, "%s.%s:%s", fqname, name, log );
        ret = LumberjackSendPairsAsLogs( hostName, port, spaceGUID, token, logs, certPath, soPath);

        DERR("SendPairsAsLogs returned: %d\r\n", ret);

        if ( logs )
            free((char*)logs);
    }

    if ( fqname )
        free((char*)fqname);

    return ret;
};
