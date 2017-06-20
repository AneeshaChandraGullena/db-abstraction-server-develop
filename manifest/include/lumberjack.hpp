/* © Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM */

#ifndef __LUMBERJACK_PROTO
#define __LUMBERJACK_PROTO

//Define MT LumberJack Protocol Supporting Lib
#include <string>
#include <vector>
#include <map>
#include <stdint.h>

#define LUMBERJACK_OK       0x0000
#define LUMBERJACK_AUTH     0x0001
#define LUMBERJACK_OVERFLOW 0x0002
#define LUMBERJACK_NOAUTH   0xFF00
#define LUMBERJACK_ERROR    0xFF01

namespace ServiceManifest {
    class TLSNetworkConnection;
};

namespace ServiceManifest {
    class MTLumberjackProtocol {
        public:
                                            MTLumberjackProtocol(
                                                const std::string& certsPath,
                                                const std::string& host,
                                                const std::string& port,
                                                const std::string& spaceGUID,
                                                const std::string& token );
            virtual                         ~MTLumberjackProtocol( void );

        private:
                                            MTLumberjackProtocol( const MTLumberjackProtocol& src );
                    MTLumberjackProtocol&   operator= (const MTLumberjackProtocol& src );

        public:
                    void                    addKeyValue(    const std::string& key, 
                                                            const std::string& value );
                    int                     sendMetrics( void );
                    int                     sendLogs( void );
    
        protected:
                    int                     connect( void );
                    int                     sendWindow( uint32_t windowSize );
                    int                     sendMetric( uint32_t current, const char* name, const char* val );
                    int                     sendLog( uint32_t current, std::map<std::string, std::string> pairs );
                    int                     acknowledgeAuth ( void );
                    int                     acknowledge ( uint32_t& sequence );

        protected:
                    TLSNetworkConnection*       m_connection;

                    std::map<std::string,
                        std::string>            m_pairs;
                    std::string                 m_host;
                    std::string                 m_port;
                    std::string                 m_spaceGUID;
                    std::string                 m_token;

    };

};
#endif //__LUMBERJACK_PROTO
