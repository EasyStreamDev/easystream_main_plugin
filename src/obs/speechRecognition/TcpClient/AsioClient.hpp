#pragma once
#include <iostream>
#include <asio.hpp>
#include <functional>
#include <atomic>
#include "../../../utils/Json.hpp"
#define MSGMAX 2048


namespace es::transcript {
    class AsioClient {
        public:
            AsioClient(const std::atomic_bool &);

            bool connectToServer();
            void readMessage();
            bool writeMessage(const std::string &);
            void setPushFunc(std::function<void (const json &)>);

        private:
            asio::io_context _ctx;
            asio::ip::tcp::socket _sock;
            char _read[MSGMAX];
            char _write[MSGMAX];
            // std::unordered_map<const char *, > _routes;
            std::function<void (const json &)> _push;
            const std::atomic_bool &_running;
    };
}