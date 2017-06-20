/* © Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM */

#include <dlfcn.h>
#include <stdio.h>
#include "servicemanifestwrapper.h"

#ifdef DEBUG
#define DERR(fmt, args...) fprintf(stderr, fmt, ## args )
#define DOUT(fmt, args...) fprintf(stdout, fmt, ## args )
#else
#define DERR(fmt, args...)
#define DOUT(fmt, args...)
#endif

typedef const char* (*func_t)(void);
typedef const char* (*func_d)(const char delimit);

typedef int         (*func_j)(const char* ca_path, const char* server, const char* host,\
                                const char* spaceGUID, const char* token, const char* pairs);

typedef int         (*func_h)(const char* ca_path, const char* host, const char* port,\
                                const char* uri, const char* headers, const char* body,\
                                unsigned char*& response );

void* getLibraryHandle( const char* lib )
{
    const char *str = "© Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM";
    void* handle = dlopen(lib, RTLD_LAZY);
    
    const char* err = dlerror();

    if ( !handle || err )
    {
        DERR("Failed to open service manifest shared object\r\n");
        DERR("Err: %s", err);
        return NULL;
    }

    return handle;
};

//TODO: Templatize
func_t getFunction( void* handle, const char* name )
{
    func_t func = (func_t) dlsym(handle, name);

    const char* err = dlerror();

    if ( !func || err )
    {
        DERR("Service manifest shared object does not contain symbol: %s \r\n",name);
        DERR("Err: %s", err);
        return NULL;
    }

    return func;
};

func_d getDFunction( void* handle, const char* name )
{
    func_d func = (func_d) dlsym(handle, name);

    const char* err = dlerror();

    if ( !func || err )
    {
        DERR("Service manifest shared object does not contain symbol: %s \r\n",name);
        DERR("Err: %s", err);
        return NULL;
    }

    return func;
};

func_j getJFunction( void* handle, const char* name )
{
    func_j func = (func_j) dlsym(handle, name);

    const char* err = dlerror();

    if ( !func || err )
    {
        DERR("Service manifest shared object does not contain symbol: %s \r\n", name);
        DERR("Err: %s", err);
        return NULL;
    }

    return func;
};

func_h getHFunction( void* handle, const char* name )
{
    func_h func = (func_h) dlsym(handle, name);

    const char* err = dlerror();

    if ( !func || err )
    {
        DERR("Service manifest shared object does not contain symbol: %s \r\n", name);
        DERR("Err: %s", err);
        return NULL;
    }

    return func;
};

bool closeLibraryHandle( void* handle )
{
    dlclose(handle);
    const char* err = dlerror();

    if ( err )
    {
        DERR("Error closing handle to service manifest library. \r\n");
        DERR("Err: %s", err);
        return false;
    }

    return true;
};


int LumberjackSendPairsAsMetrics(
            const char* host,
            const char* port,
            const char* spaceGUID,
            const char* token,
            const char* sendPairs,
            const char* ca_path,
            const char* libpath )
{
    void* handle = getLibraryHandle(libpath);

    if ( !handle )
    {
        return -1;
    }

    func_j func = getJFunction(handle, "LumberjackSendPairsAsMetrics");

    if ( !func )
    {
        return -1;
    }

    int ret = func ( ca_path, host, port, spaceGUID, token, sendPairs );

    closeLibraryHandle(handle);

    return ret;
};

int LumberjackSendPairsAsLogs(
            const char* host,
            const char* port,
            const char* spaceGUID,
            const char* token,
            const char* sendPairs,
            const char* ca_path,
            const char* libpath )
{
    void* handle = getLibraryHandle(libpath);

    if ( !handle )
    {
        return -1;
    }

    func_j func = getJFunction(handle, "LumberjackSendPairsAsLogs");

    if ( !func )
    {
        return -1;
    }

    int ret = func ( ca_path, host, port, spaceGUID, token, sendPairs );

    closeLibraryHandle(handle);

    return ret;
};

int HTTPGet (   const char* host,
                const char* port,
                const char* uri,
                const char* headers,
                const char* body,
                unsigned char** response,
                const char* ca_path,
                const char* libpath )
{
    void* handle = getLibraryHandle(libpath);

    if ( !handle )
    {
        return -1;
    }

    func_h func = getHFunction(handle, "HTTPGet");

    if ( !func )
    {
        return -1;
    }

    int ret = func ( ca_path, host, port, uri, headers, body, *response );

    closeLibraryHandle(handle);

    return ret;
}

int HTTPPost (  const char* host,
                const char* port,
                const char* uri,
                const char* headers,
                const char* body,
                unsigned char** response,
                const char* ca_path,
                const char* libpath )
{
    void* handle = getLibraryHandle(libpath);

    if ( !handle )
    {
        return -1;
    }

    func_h func = getHFunction(handle, "HTTPPost");

    if ( !func )
    {
        return -1;
    }

    int ret = func ( ca_path, host, port, uri, headers, body, *response );

    closeLibraryHandle(handle);

    return ret;
}

int HTTPPut (   const char* host,
                const char* port,
                const char* uri,
                const char* headers,
                const char* body,
                unsigned char** response,
                const char* ca_path,
                const char* libpath )
{
    void* handle = getLibraryHandle(libpath);

    if ( !handle )
    {
        return -1;
    }

    func_h func = getHFunction(handle, "HTTPPut");

    if ( !func )
    {
        return -1;
    }

    int ret = func ( ca_path, host, port, uri, headers, body, *response );

    closeLibraryHandle(handle);

    return ret;
}

int HTTPDelete ( const char* host,
                    const char* port,
                    const char* uri,
                    const char* headers,
                    const char* body,
                    unsigned char** response,
                    const char* ca_path,
                    const char* libpath )
{
    void* handle = getLibraryHandle( libpath );

    if ( !handle )
    {
        return -1;
    }

    func_h func = getHFunction(handle, "HTTPDelete");

    if ( !func )
    {
        return -1;
    }

    int ret = func ( ca_path, host, port, uri, headers, body, *response );

    closeLibraryHandle(handle);

    return ret;
}

int HTTPOptions ( const char* host,
                const char* port,
                const char* uri,
                const char* headers,
                const char* body,
                unsigned char** response,
                const char* ca_path,
                const char* libpath )
{
    void* handle = getLibraryHandle(libpath);

    if ( !handle )
    {
        return -1;
    }

    func_h func = getHFunction(handle, "HTTPOptions");

    if ( !func )
    {
        return -1;
    }

    int ret = func ( ca_path, host, port, uri, headers, body, *response );

    closeLibraryHandle(handle);

    return ret;
}

const char* getCatalogServiceName( const char* libpath )
{
    void* handle = getLibraryHandle(libpath);

    if ( !handle )
    {
        return NULL;
    }

    const char* (*func)() = getFunction(handle, "getCatalogServiceName");

    if ( !func )
    {
        return NULL;
    }

    const char* res = func();

    closeLibraryHandle(handle);

    return res;
};

const char* getComponentName ( const char* libpath )
{
    void* handle = getLibraryHandle(libpath);

    if ( !handle )
    {
        return NULL;
    }

    const char* (*func)() = getFunction(handle, "getComponentName");

    if (!func)
    {
        return NULL;
    }

    const char* res = func();

    closeLibraryHandle(handle);

    return res;
};

const char*  getResourceType( const char* libpath )
{
    void* handle = getLibraryHandle( libpath );

    if ( !handle )
    {
        return NULL;
    }

    const char* (*func)() = getFunction(handle, "getResourceType");

    if ( !func )
    {
        return NULL;
    }

    const char* res = func();

    closeLibraryHandle(handle);

    return res;
};

const char*  getResourceName( const char* libpath )
{
    void* handle = getLibraryHandle(libpath);

    if ( !handle )
    {
        return NULL;
    }

    const char* (*func)() = getFunction(handle, "getResourceName");

    if ( !func )
    {
        return NULL;
    }

    const char* res = func();

    closeLibraryHandle(handle);

    return res;
};

const char*  getPagerDutyTeamURL(const char* libpath)
{
    void* handle = getLibraryHandle(libpath);

    if ( !handle )
    {
        return NULL;
    }

    const char* (*func)() = getFunction(handle, "getPagerDutyTeamURL");

    if ( !func )
    {
        return NULL;
    }

    const char* res = func();

    closeLibraryHandle(handle);

    return res;
};

const char*  getBaileyProjectName( const char* libpath )
{
    void* handle = getLibraryHandle( libpath );

    if ( !handle )
    {
        return NULL;
    }

    const char* (*func)() = getFunction(handle, "getBaileyProjectName");

    if ( !func )
    {
        return NULL;
    }

    const char* res = func();

    closeLibraryHandle(handle);

    return res;
};

const char*  getBaileyInstanceURL( const char* libpath )
{
    void* handle = getLibraryHandle( libpath );

    if ( !handle )
    {
        return NULL;
    }

    const char* (*func)() = getFunction(handle, "getBaileyInstanceURL");

    if ( !func )
    {
        return NULL;
    }

    const char* res = func();

    closeLibraryHandle(handle);

    return res;
};

const char*  getTenancyModel( const char* libpath )
{
    void* handle = getLibraryHandle( libpath );

    if ( !handle )
    {
        return NULL;
    }

    const char* (*func)() = getFunction(handle, "getTenancyModel");

    if ( !func )
    {
        return NULL;
    }

    const char* res = func();

    closeLibraryHandle(handle);

    return res;
};

const char*  getTeamEmailAddress( const char* libpath )
{
    void* handle = getLibraryHandle( libpath );

    if ( !handle )
    {
        return NULL;
    }

    const char* (*func)() = getFunction(handle, "getTeamEmailAddress");

    if ( !func )
    {
        return NULL;
    }

    const char* res = func();

    closeLibraryHandle(handle);

    return res;
};

const char*  getInstanceId( const char* libpath )
{
    void* handle = getLibraryHandle( libpath );

    if ( !handle )
    {
        return NULL;
    }

    const char* (*func)() = getFunction(handle, "getInstanceId");

    if ( !func )
    {
        return NULL;
    }

    const char* res = func();

    closeLibraryHandle(handle);

    return res;
};

const char*  getSourceCodeRepoURL( const char* libpath )
{
    void* handle = getLibraryHandle( libpath );

    if ( !handle )
    {
        return NULL;
    }

    const char* (*func)() = getFunction(handle, "getSourceCodeRepoURL");

    if ( !func )
    {
        return NULL;
    }

    const char* res = func();

    closeLibraryHandle(handle);

    return res;
};

const char*  getCRN( const char* libpath, const char delimit )
{
    void* handle = getLibraryHandle( libpath );

    if ( !handle )
    {
        return NULL;
    }

    func_d func = getDFunction(handle, "getCRN");

    if ( !func )
    {
        return NULL;
    }

    const char* res = func(delimit);

    closeLibraryHandle(handle);

    return res;
};

const char* getFQComponentName( const char* libpath, const char delimit )
{
    void* handle = getLibraryHandle(libpath);

    if ( !handle )
    {
        return NULL;
    }

    func_d func = getDFunction(handle, "getFQComponentName");

    if ( !func )
    {
        return NULL;
    }

    const char* res = func(delimit);

    closeLibraryHandle( handle );

    return res;
};

const char* getManifestJSON( const char* libpath )
{
    void* handle = getLibraryHandle( libpath );

    if ( !handle )
    {
        return NULL;
    }

    const char* (*func)() = getFunction(handle, "getManifestJSON");

    if ( !func )
    {
        return NULL;
    }

    const char* res = func();

    closeLibraryHandle(handle);

    return res;
};
