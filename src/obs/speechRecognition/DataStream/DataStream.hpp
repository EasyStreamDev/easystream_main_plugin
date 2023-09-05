#pragma once

#include <iostream>
#include <string>
#include <mutex>
#include <atomic>

#ifdef unix
#include <unistd.h>
#include <arpa/inet.h>
#elif _WIN32
#include <WinSock2.h>
#endif

namespace es::transcript
{
    class DataStream
    {
    public:
        DataStream();
        ~DataStream();

        bool connectToTranscription();
        bool sendMessage(std::string);
        // bool sendMessage(const uint);
        void setPort(int);
        int getPort() const;
        void disconnectSocket();

    private:
        std::atomic_int _port;
        std::string _queue;
        std::mutex _mtx;
        std::atomic_bool _closed;
#ifdef unix
        int _sock = 0;
#elif _WIN32
        SOCKET _sock;
        WSADATA _wsaData;
#endif
    };
}