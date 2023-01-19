/*
** EPITECH PROJECT, 2021
** NetworkCommunication
** File description:
** AsioTcpServer
*/

#ifndef ASIOTCPSERVER_HPP_
#define ASIOTCPSERVER_HPP_
// #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define BOOST_ASIO_DISABLE_IOCP 1
// static const std::string slash="\\";
// static const std::string slash="/";
// #endif

#include <boost/thread.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <boost/make_shared.hpp>

#include "../../plugin-main.hpp"
#include "../../nlohmann/json.hpp"
#include "AsioTcpConnection.hpp"
#include "errorCode.hpp"
#include "common_using.hpp"

namespace es::server
{
    class AsioTcpServer : public boost::enable_shared_from_this<AsioTcpServer>
    {
        /***********/
        /* METHODS */
        /***********/
    public:
        AsioTcpServer(const std::string &host, int port, const std::unordered_map<std::string, std::shared_ptr<obs::AutoAudioLeveler>> &);
        ~AsioTcpServer() = default;

        // --- Network
        bool start();
        void writeMessage(const std::string &);
        void update();

        // --- Getters
        boost::asio::io_context &getContext();

    private:
        // --- Network
        void waitForClientConnection();

        // --- GET requests
        void getAllMics(const json &, Shared<AsioTcpConnection> &);
        void getActReactCouples(const json &, Shared<AsioTcpConnection> &);

        // --- SET requests
        void setAutoAudioLeveler(const json &, Shared<AsioTcpConnection> &);
        void setMicLevel(const json &, Shared<AsioTcpConnection> &);

        // --- REMOVE requests
        void removeActReact(const json &, Shared<AsioTcpConnection> &);

        // --- BAD requests
        void badCommand(Shared<AsioTcpConnection> &);
        void generateMobileInformation(){};

        /********************/
        /* MEMBER VARIABLES */
        /********************/
    private:
        // --- Thread
        boost::thread _threadContext;
        // --- Network
        boost::asio::io_context _ioContext;
        boost::asio::ip::tcp::acceptor _acceptor;
        // boost::asio::ip::tcp::endpoint _endPoint;
        std::vector<Shared<AsioTcpConnection>> _connections;
        // --- Request handler vars
        std::unordered_map<std::string, void (AsioTcpServer::*)(const json &, Shared<AsioTcpConnection> &)> _handler;
        const std::unordered_map<std::string, std::shared_ptr<obs::AutoAudioLeveler>> &_audioLeveler;

        // TMP - @todo : use a shared map with mutex
        std::unordered_map<size_t, area::area_t> areas;
    };
}

#endif /* !ASIOTCPSERVER_HPP_ */
