/* © Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM */

#ifndef __SERVICE_MANIFEST_STUB__
#define __SERVICE_MANIFEST_STUB__
#include "servicemanifest.hpp"

extern "C" const char*  getCatalogServiceName(void);
extern "C" const char*  getComponentName(void);
extern "C" const char*  getResourceType(void);
extern "C" const char*  getResourceName(void);
extern "C" const char*  getPagerDutyTeamURL(void);
extern "C" const char*  getBaileyProjectName(void);
extern "C" const char*  getBaileyInstanceURL(void);
extern "C" const char*  getTenancyModel(void);
extern "C" const char*  getTeamEmailAddress(void);
extern "C" const char*  getInstanceId(void);
extern "C" const char*  getSourceCodeRepoURL(void);
extern "C" const char*  getCRN( const char delimit = ':');
extern "C" const char*  getManifestJSON(void);
extern "C" const char*  getFQComponentName( const char delimit = ':' );
#endif
