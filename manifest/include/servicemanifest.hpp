/* © Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM */

#ifndef __SERVICEMANIFEST_H__
#define __SERVICEMANIFEST_H__
#include <string>

namespace ServiceManifest {
    class JSONSchema_Driver;
};

namespace ServiceManifest
{
    class ServiceManifest
    {
        public:
                            ServiceManifest( const char* manifest_start, const char* manifest_end );
                            ~ServiceManifest( );

        private:
                            ServiceManifest( const ServiceManifest& src ); // Hide copy CTOR;
                            ServiceManifest &operator= ( const ServiceManifest& src ); // Hide assignment OTOR

        private:
                            JSONSchema_Driver*   smParser;

        public:

                std::string                 getCatalogServiceName(void);
                std::string                 getComponentName(void);
                std::string                 getCName(void);
                std::string                 getCType(void);
                std::string                 getResourceType(void);
                std::string                 getResourceName(void);
                std::string                 getPagerDutyTeamURL(void);
                std::string                 getBaileyProjectName(void);
                std::string                 getBaileyInstanceURL(void);
                std::string                 getTenancyModel(void);
                std::string                 getTeamEmailAddress(void);
                std::string                 getInstanceId(void);
                std::string                 getSourceCodeRepoURL(void);
                std::string                 getRegion(void);
                std::string                 getScope(void);
                std::string                 getCRN(const char delimit);
                std::string                 getFQComponentName(const char delimit);
                std::string                 getManifestJSON(void);
    };
};
#endif //__SERVICEMANIFEST_H__
