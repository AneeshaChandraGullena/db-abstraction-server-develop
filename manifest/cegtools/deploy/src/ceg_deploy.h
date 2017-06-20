/* © Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM */

#ifndef __MAIN_H
#define __MAIN_H


//MCB commands
const char* Log                     = "cegLog";
const char* Instrument              = "cegInstrument";
const char* Register                = "cegRegister";

//Build Metrics and logs
const char* cegrecordbuild          = "cegRecordBuild";
const char* ceglogbuild             = "cegLogBuild";

//Deploy Metrics and logs
const char* cegrecorddeploystart    = "cegRecordDeployStart";
const char* cegrecorddeployend      = "cegRecordDeployEnd";
const char* cegstartpredeploy       = "cegStartPreDeploy";
const char* cegendpredeploy         = "cegEndPreDeploy";
const char* cegrecordchangelog      = "cegRecordChangeLog";

//Service Registry
const char* cegRegisterGet          = "get";
const char* cegRegisterPost         = "post";
const char* cegRegisterPut          = "put";

int recordBuildWithMetric( const char* name, const char* value );
int recordBuildWithLog( const char* name, const char* log );

int getManifestFromRegistry( void );
int postManifestToRegistry( void );
int putManifestToRegistry(void);

const char* copySourceAndReplaceCommasAndColons( const char* source );
const char* urlEncode( const char* value );

int main ( int argc, char** argv );

#endif
