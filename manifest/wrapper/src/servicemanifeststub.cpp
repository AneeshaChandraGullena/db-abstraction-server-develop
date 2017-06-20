/* © Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM */

#include <string>
#include <iostream>
#include <string.h>
#include "servicemanifeststub.h"

extern const char manifest_data[]     asm("_binary____res_servicemanifest_json_start");
extern const char manifest_data_end[] asm("_binary____res_servicemanifest_json_end");
//extern const char manifest_data[]       asm("_binary_______servicemanifest_json_start");
//extern const char manifest_data_end[]   asm("_binary_______servicemanifest_json_end");

const char* getCatalogServiceName(void)
{
    ServiceManifest::ServiceManifest* manifest = new ServiceManifest::ServiceManifest(manifest_data, manifest_data_end);
    const char* result = strdup(manifest->getCatalogServiceName().c_str());

    delete manifest;

    return result; //CALLER MUST FREE
};

const char*  getComponentName(void)
{
    ServiceManifest::ServiceManifest* manifest = new ServiceManifest::ServiceManifest(manifest_data, manifest_data_end);
    const char* result = strdup(manifest->getComponentName().c_str());

    delete manifest;

    return result; //CALLER MUST FREE
};

const char*  getResourceType(void)
{
    ServiceManifest::ServiceManifest* manifest = new ServiceManifest::ServiceManifest(manifest_data, manifest_data_end);
    const char* result = strdup(manifest->getResourceType().c_str());

    delete manifest;

    return result; //CALLER MUST FREE
};

const char*  getResourceName(void)
{
    ServiceManifest::ServiceManifest* manifest = new ServiceManifest::ServiceManifest(manifest_data, manifest_data_end);
    const char* result = strdup(manifest->getResourceName().c_str());

    delete manifest;

    return result; //CALLER MUST FREE
};

const char*  getPagerDutyTeamURL(void)
{
    ServiceManifest::ServiceManifest* manifest = new ServiceManifest::ServiceManifest(manifest_data, manifest_data_end);
    const char* result = strdup(manifest->getPagerDutyTeamURL().c_str());

    delete manifest;

    return result; //CALLER MUST FREE
};

const char*  getBaileyProjectName(void)
{
    ServiceManifest::ServiceManifest* manifest = new ServiceManifest::ServiceManifest(manifest_data, manifest_data_end);
    const char* result = strdup(manifest->getBaileyProjectName().c_str());

    delete manifest;

    return result; //CALLER MUST FREE
};

const char*  getBaileyInstanceURL(void)
{
    ServiceManifest::ServiceManifest* manifest = new ServiceManifest::ServiceManifest(manifest_data, manifest_data_end);
    const char* result = strdup(manifest->getBaileyInstanceURL().c_str());

    delete manifest;

    return result; //CALLER MUST FREE
};

const char*  getTenancyModel(void)
{
    ServiceManifest::ServiceManifest* manifest = new ServiceManifest::ServiceManifest(manifest_data, manifest_data_end);
    const char* result = strdup(manifest->getTenancyModel().c_str());

    delete manifest;

    return result; //CALLER MUST FREE
};

const char*  getTeamEmailAddress(void)
{
    ServiceManifest::ServiceManifest* manifest = new ServiceManifest::ServiceManifest(manifest_data, manifest_data_end);
    const char* result = strdup(manifest->getTeamEmailAddress().c_str());

    delete manifest;

    return result; //CALLER MUST FREE
};

const char*  getInstanceId(void)
{
    ServiceManifest::ServiceManifest* manifest = new ServiceManifest::ServiceManifest(manifest_data, manifest_data_end);
    const char* result = strdup(manifest->getInstanceId().c_str());

    delete manifest;

    return result; //CALLER MUST FREE
};

const char*  getSourceCodeRepoURL(void)
{
    ServiceManifest::ServiceManifest* manifest = new ServiceManifest::ServiceManifest(manifest_data, manifest_data_end);
    const char* result = strdup(manifest->getSourceCodeRepoURL().c_str());

    delete manifest;

    return result; //CALLER MUST FREE
};

const char*  getCRN( const char delimit)
{
    ServiceManifest::ServiceManifest* manifest = new ServiceManifest::ServiceManifest(manifest_data, manifest_data_end);
    const char* result = strdup(manifest->getCRN(delimit).c_str());

    delete manifest;

    return result; //CALLER MUST FREE
};

const char* getManifestJSON( void )
{
    const char *str = "© Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM";

    ServiceManifest::ServiceManifest* manifest = new ServiceManifest::ServiceManifest(manifest_data, manifest_data_end);
    const char* result = strdup(manifest->getManifestJSON().c_str());

    delete manifest;

    return result; //CALLER MUST FREE
};

const char* getFQComponentName( const char delimit )
{
    ServiceManifest::ServiceManifest* manifest = new ServiceManifest::ServiceManifest(manifest_data, manifest_data_end);
    const char* result = strdup(manifest->getFQComponentName(delimit).c_str());

    delete manifest;

    return result; //CALLER MUST FREE
};
