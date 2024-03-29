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
#include "../../IPluginManager.hpp"
#include "../../utils/Obs.hpp"
#include "../../utils/types/ThreadSafeQueue.hpp"
#include "../../area/AreaManager.hpp"
#include "../../obs/autoAudioLeveler/AutoAudioLeveler.hpp"
#include "../../obs/subtitles/SubtitlesManager.hpp"
#include "../../obs/profile/UserProfile.hpp"
#include "../../obs/topAudioMic/TopAudioMic.hpp"

// Local
#include "interface/IServer.hpp"
#include "AsioTcpConnection.hpp"
#include "ResponseGenerator.hpp"
#include "CommunicationErrors.hpp"
#include "common_using.hpp"

// Linked
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <thread>
#include <future>

namespace es::server
{
    using TSRequestQueue = ThreadSafeQueue<std::pair<Shared<AsioTcpConnection>, json>>;

    class AsioTcpServer
        : public Runnable,
          public IServer,
          std::enable_shared_from_this<AsioTcpServer>
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
            {"START_REC", es::area::ReactionType::START_RECORDING},
            {"STOP_REC", es::area::ReactionType::STOP_RECORDING},
            {"START_STREAM", es::area::ReactionType::START_STREAMING},
            {"STOP_STREAM", es::area::ReactionType::STOP_STREAMING},
            {"TOGGLE_AUDIO_COMPRESSOR", es::area::ReactionType::TOGGLE_AUDIO_COMPRESSOR},
        };

        /***********/
        /* METHODS */
        /***********/
    public:
        AsioTcpServer(const std::string &, int, es::IPluginManager *);
        ~AsioTcpServer();

        void run(void *) override;

        // --- Network
        bool start(void) final;
        void update(void);
        void submitBroadcast(const json &) final;

        // --- Getters
        asio::io_context &getContext(void) final;

    private:
        // --- Network
        // void _writeMessage(const std::string &);
        void _waitForClientConnection(void);
        void _runRequestHandler(void *);
        void _executeRequest(const json &, Shared<AsioTcpConnection>);

        /* REQUESTS (@note: Methods called by receiving a specific request start with `r_`) */
        // --- GET requests
        void r_GetAllMics(const json &, Shared<AsioTcpConnection>);
        void r_GetAllScenes(const json &, Shared<AsioTcpConnection>);
        void r_GetAllTextFields(const json &, Shared<AsioTcpConnection>);
        void r_GetAllDisplaySources(const json &, Shared<AsioTcpConnection>);
        void r_GetActReactCouples(const json &, Shared<AsioTcpConnection>);
        void r_GetProfileSettings(const json &, Shared<AsioTcpConnection>);
        void r_GetSubtitlesSettings(const json &, Shared<AsioTcpConnection>);
        void r_GetAllLinksMicsToDisplaySources(const json &, Shared<AsioTcpConnection>);
        void r_broadcastArea();
        // --- SET requests
        void r_SetCompressorLevel(const json &, Shared<AsioTcpConnection>);
        void r_SetNewARea(const json &, Shared<AsioTcpConnection>);
        void r_SetSubtitles(const json &, Shared<AsioTcpConnection>);
        void r_LinkMicToDisplaySources(const json &, Shared<AsioTcpConnection>);
        // --- UPDATE requests
        void r_UpdateAction(const json &, Shared<AsioTcpConnection>);
        void r_UpdateReaction(const json &j, Shared<AsioTcpConnection> con);
        // --- REMOVE requests
        void r_RemoveActReact(const json &, Shared<AsioTcpConnection>);
        void r_UnlinkMicToDisplaySources(const json &, Shared<AsioTcpConnection>);
        // --- SUBSCRIPTION requests
        void r_SubscribeToBroadcast(const json &, Shared<AsioTcpConnection>);

        // --- MISCELLANEOUS
        void _generateMobileInformation(){};

        // --- BROADCAST
        void _broadcast(const json &);

        /********************/
        /* MEMBER VARIABLES */
        /********************/

    private:
        // --- Plugin manager
        es::IPluginManager *m_PluginManager;
        // --- Thread
        std::thread m_ThreadContext;
        std::thread m_RequestHandler;
        // --- Network
        asio::io_context m_IoContext;
        asio::ip::tcp::acceptor m_Acceptor;
        // boost::asio::ip::tcp::endpoint _endPoint;
        std::vector<Shared<AsioTcpConnection>> m_Connections;
        // --- Request handler vars
        TSRequestQueue m_InRequestQueue;
        TSRequestQueue m_OutRequestQueue;
        ThreadSafeQueue<json> m_BroadcastQueue;
        std::unordered_map<std::string, void (AsioTcpServer::*)(const json &, Shared<AsioTcpConnection>)> m_Handler;
    };
}

#endif /* !ASIOTCPSERVER_HPP_ */
