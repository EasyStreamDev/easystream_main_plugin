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
#include "AsioTcpConnection.hpp"
#include <boost/thread.hpp>
#include "common_using.hpp"
#include "../../plugin-main.hpp"
#include "errorCode.hpp"
#include "../../nlohmann/json.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <boost/make_shared.hpp>

namespace es::server
{
    enum TriggerType
    {
        APP_CHANGE = 0,
        KEY_WORD = 1
    };

    typedef struct
    {
        // Trigger type.
        TriggerType type;
        // Trigger value (ex: is type is TriggerType::KEY_WORD, value is the trigger word).
        std::string value;
        // Scene to change to when triggered.
        std::string target_scene_name;
    } st_instruction_t;

    class AsioTcpServer : public boost::enable_shared_from_this<AsioTcpServer>
    {
    public:
        /***********/
        /* Methods */
        /***********/

        AsioTcpServer(const std::string &host, int port, const std::unordered_map<std::string, std::shared_ptr<obs::AutoAudioLeveler>> &);
        ~AsioTcpServer();

        // --- NETWORK
        bool start();
        void writeMessage(const std::string &);
        void update();

        // --- GETTER
        boost::asio::io_context &getContext();

    private:
        // --- NETWORK
        void waitForClientConnection();

        // --- GET REQUESTS
        void getAllMics(const json &, Shared<AsioTcpConnection> &);
        void getActReactCouples(const json &, Shared<AsioTcpConnection> &);

        // --- SET REQUESTS
        void setAutoAudioLeveler(const json &, Shared<AsioTcpConnection> &);
        void setMicLevel(const json &, Shared<AsioTcpConnection> &);
        void setSceneSwapTrigger(const json &, Shared<AsioTcpConnection> &);

        // --- REMOVE REQUESTS
        void removeActReact(const json &, Shared<AsioTcpConnection> &);

        // --- BAD REQUESTS
        void badCommand(Shared<AsioTcpConnection> &);
        void generateMobileInformation();

        /********************/
        /* Member variables */
        /********************/

        // --- Thread
        boost::thread _threadContext;
        // --- Network
        boost::asio::io_context _ioContext;
        boost::asio::ip::tcp::acceptor _acceptor;
        // boost::asio::ip::tcp::endpoint _endPoint;
        std::vector<Shared<AsioTcpConnection>> _connections;
        // --- Request handler vars
        std::unordered_map<std::string, void (AsioTcpServer::*)(const json &, Shared<AsioTcpConnection> &)> _handler;
        std::vector<st_instruction_t> _sceneSwapTriggers;
        const std::unordered_map<std::string, std::shared_ptr<obs::AutoAudioLeveler>> &_audioLeveler;

        // TMP - @todo : use a shared map with mutex
        std::unordered_map<size_t, area::area_t> areas;
    };
}

#endif /* !ASIOTCPSERVER_HPP_ */
