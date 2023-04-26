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

// Global
#include "../../Runnable.hpp"
#include "../../utils/Obs.hpp"
#include "../../area/AreaManager.hpp"
#include "../../obs/autoAudioLeveler/AutoAudioLeveler.hpp"
#include "../../IPluginManager.hpp"

// Local
#include "AsioTcpConnection.hpp"
#include "errorCode.hpp"
#include "common_using.hpp"
#include "TSRequestQueue.hpp"

// Linked
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <thread>

namespace es::server
{
    class AsioTcpServer : public std::enable_shared_from_this<AsioTcpServer>, Runnable
    {
        /*********************/
        /* USEFULL CONSTANTS */
        /*********************/
    public:
        // Mapping of std::string to enum on the different action types
        const std::unordered_map<std::string, es::area::ActionType> ACTION_TYPE_MAP = {
            {"WORD_DETECT", es::area::ActionType::WORD_DETECT},
            {"APP_LAUNCH", es::area::ActionType::APP_LAUNCH},
            {"KEY_PRESSED", es::area::ActionType::KEY_PRESSED},
        };
        // Mapping of std::string to enum on the different reaction types
        const std::unordered_map<std::string, es::area::ReactionType> REACTION_TYPE_MAP = {
            {"SCENE_SWITCH", es::area::ReactionType::SCENE_SWITCH},
            {"TOGGLE_AUDIO_COMPRESSOR", es::area::ReactionType::TOGGLE_AUDIO_COMPRESSOR},
        };

        /***********/
        /* METHODS */
        /***********/
    public:
        AsioTcpServer(const std::string &, int, es::IPluginManager *);
        ~AsioTcpServer();

        void run(void *) override;
        void runRequestHandler(void *);
        void handleRequest(const json &, Shared<AsioTcpConnection>);

        // --- Network
        bool start();
        void writeMessage(const std::string &);
        void update();

        // --- Getters
        asio::io_context &getContext();

    private:
        // --- Network
        void waitForClientConnection();

        // --- GET requests
        void getAllMics(const json &, Shared<AsioTcpConnection> &);
        void getActReactCouples(const json &, Shared<AsioTcpConnection> &);

        // --- SET requests
        void setAutoAudioLeveler(const json &, Shared<AsioTcpConnection> &);
        void setMicLevel(const json &, Shared<AsioTcpConnection> &);
        void setNewARea(const json &, Shared<AsioTcpConnection> &);

        // --- UPDATE requests
        void updateAction(const json &, Shared<AsioTcpConnection> &);
        void updateReaction(const json &j, Shared<AsioTcpConnection> &con);

        // --- REMOVE requests
        void removeActReact(const json &, Shared<AsioTcpConnection> &);

        // --- RESPONSES GENERATORS
        static const json responseSuccess(const std::string & = "", const json & = {});
        static const json badCommand(void);
        static const json badRequest(const std::string & = "");
        static const json notFound(const std::string & = "");

        // --- MISCELLANEOUS
        void generateMobileInformation(){};

        /********************/
        /* MEMBER VARIABLES */
        /********************/
    private:
        // --- Plugin manager
        es::IPluginManager *m_PluginManager;
        // --- Thread
        std::thread _threadContext;
        std::thread m_RequestHandler;
        // --- Network
        asio::io_context _ioContext;
        asio::ip::tcp::acceptor _acceptor;
        // boost::asio::ip::tcp::endpoint _endPoint;
        std::vector<Shared<AsioTcpConnection>> _connections;
        // --- Request handler vars
        TSRequestQueue m_InRequestQueue;
        TSRequestQueue m_OutRequestQueue;
        std::unordered_map<std::string, void (AsioTcpServer::*)(const json &, Shared<AsioTcpConnection> &)> _handler;
    };
}

#endif /* !ASIOTCPSERVER_HPP_ */
