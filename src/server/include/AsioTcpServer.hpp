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

// Linked
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
// --- Boost
#include <boost/thread.hpp>
#include <boost/make_shared.hpp>

namespace es::server
{
    class AsioTcpServer : public boost::enable_shared_from_this<AsioTcpServer>, Runnable
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
        ~AsioTcpServer() = default;

        void run(void *) override;

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
        void setNewARea(const json &, Shared<AsioTcpConnection> &);

        // --- UPDATE requests
        void updateAction(const json &, Shared<AsioTcpConnection> &);
        void updateReaction(const json &j, Shared<AsioTcpConnection> &con);

        // --- REMOVE requests
        void removeActReact(const json &, Shared<AsioTcpConnection> &);

        // --- RESPONSES
        void sendSuccess(Shared<AsioTcpConnection> &, const std::string & = "", const json & = {});
        void badCommand(Shared<AsioTcpConnection> &);
        void badRequest(Shared<AsioTcpConnection> &, const std::string & = "");
        void notFound(Shared<AsioTcpConnection> &, const std::string & = "");

        // --- MISCELLANEOUS
        void generateMobileInformation(){};

        /********************/
        /* MEMBER VARIABLES */
        /********************/
    private:
        // --- Plugin manager
        es::IPluginManager *m_PluginManager;
        // --- Thread
        boost::thread _threadContext;
        // --- Network
        boost::asio::io_context _ioContext;
        boost::asio::ip::tcp::acceptor _acceptor;
        // boost::asio::ip::tcp::endpoint _endPoint;
        std::vector<Shared<AsioTcpConnection>> _connections;
        // --- Request handler vars
        std::unordered_map<std::string, void (AsioTcpServer::*)(const json &, Shared<AsioTcpConnection> &)> _handler;
    };
}

#endif /* !ASIOTCPSERVER_HPP_ */
