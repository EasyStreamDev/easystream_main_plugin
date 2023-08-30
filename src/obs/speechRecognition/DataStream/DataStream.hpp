#pragma once

#include <iostream>
#include <string>

#ifdef linux
    #include <unistd.h>
    #include <arpa/inet.h>
#elif _WIN32
    #include <WinSock2.h>
#endif

namespace es::transcript
{
    class DataStream {
        public:
            DataStream();
            ~DataStream();
            
            bool connectToTranscription();
            bool sendMessage(const char *);
            // bool sendMessage(const uint);
            void setPort(int);
            int getPort() const;
        private:
            int _port;
            #ifdef linux
                int _sock = 0;
            #elif _WIN32
                SOCKET _sock;
                WSADATA _wsaData;
                bool _closed = true;
                std::string _queue;
            #endif

    };
}