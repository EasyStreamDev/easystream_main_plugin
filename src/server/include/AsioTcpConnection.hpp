/*
** EPITECH PROJECT, 2021
** NetworkCommunication
** File description:
** AsioTcpConnection
*/

#ifndef ASIOTCPCONNECTION_HPP_
#define ASIOTCPCONNECTION_HPP_

#define MSGMAX 2048

// Global
#include "../../utils/Json.hpp"

// Linked
// #include <boost/asio.hpp>
// #include <boost/enable_shared_from_this.hpp>
#include <asio.hpp>
#include <iostream>

namespace es
{
    namespace server
    {
        class AsioTcpConnection : public std::enable_shared_from_this<AsioTcpConnection>
        {
        public:
            AsioTcpConnection(asio::ip::tcp::socket &sock);
            ~AsioTcpConnection();

            void readMessage();
            void writeMessage(const std::string &msg);
            const asio::ip::tcp::socket &getSocket() const;
            const std::vector<nlohmann::json> getMessage();
            bool isConnected() const;
            inline void setBroadcastSubscription(const bool &v) { _subscribedToBroadcast = v; }
            inline const bool &isSubscribedToBroadcast(void) const { return _subscribedToBroadcast; }

        private:
            bool _connected;
            char _buffer[MSGMAX];
            char _receiver[MSGMAX];
            bool _subscribedToBroadcast = false;
            asio::ip::tcp::socket _socket;
            std::vector<nlohmann::json> _messages;
            std::mutex _writeMutex;
        };
    }
}

#endif /* !ASIOTCPCONNECTION_HPP_ */
