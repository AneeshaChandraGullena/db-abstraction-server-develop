/* © Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM */

#ifndef __SIMPLEHTTP_PROTO
#define __SIMPLEHTTP_PROTO

//Define HTTP/REST Protocol Supporting Lib
#include <string>
#include <vector>
#include <map>
#include <stdint.h>

#define HTTP_OK         0x0000
#define HTTP_ERROR      0x0001

namespace ServiceManifest {
    class TLSNetworkConnection;
};

namespace ServiceManifest {
    class SimpleHTTP {

        public:
                                            SimpleHTTP(
                                                const std::string& certsPath,
                                                const std::string& host,
                                                const std::string& port );

            virtual                         ~SimpleHTTP( void );

        private:
                                            SimpleHTTP( const SimpleHTTP& src );
                    SimpleHTTP&     operator= (const SimpleHTTP& src );

        public:
                    void                    addHeader ( const std::string& name,
                                                        const std::string& value );

                    int                     GET     (   const std::string& uri,
                                                        const std::string& body,
                                                        unsigned char*& responseBuffer );
                    int                     POST    (   const std::string& uri,
                                                        const std::string& body,
                                                        unsigned char*& responseBuffer );
                    int                     PUT     (   const std::string& uri,
                                                        const std::string& body,
                                                        unsigned char*& responseBuffer );
                    int                     DELETE  (   const std::string& uri,
                                                        const std::string& body,
                                                        unsigned char*& responseBuffer );
                    int                     OPTIONS (   const std::string& uri,
                                                        const std::string& body,
                                                        unsigned char*& responseBuffer );
        protected:
                    int                     parseResponseCode ( const unsigned char* responseBuffer );
                    int                     send    (   const std::string& uri,
                                                        const char* verb,
                                                        const std::map<std::string, std::string>& headers,
                                                        const std::string& body,
                                                        unsigned char*& responseBuffer );

        protected:
                    TLSNetworkConnection*       m_connection;

                    std::map<std::string,
                        std::string>            m_headers;
                    std::string                 m_host;
                    std::string                 m_port;

        private:    
                   static const char* const    Version;

    };

};
#endif //__SIMPLEHTTP_PROTO
