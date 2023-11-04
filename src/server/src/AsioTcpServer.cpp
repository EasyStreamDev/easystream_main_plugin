/*
** NetworkCommunication
** File description:
** AsioTcpServer
*/

#include "../include/AsioTcpServer.hpp"

namespace es::server
{
    AsioTcpServer::AsioTcpServer(
        const std::string &host,
        int port,
        es::IPluginManager *pm)
        : m_PluginManager(pm),
          /*_endPoint(boost::asio::ip::make_address(host), port),*/
          m_Acceptor(m_IoContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    {
        /* Easystream routes */
        // --- Microphones / Auto leveler
        m_Handler["/microphones/get"] = &AsioTcpServer::r_GetAllMics;
        m_Handler["/microphones/auto-leveler/set"] = &AsioTcpServer::r_SetCompressorLevel;
        // --- Action -> Reaction couples
        m_Handler["/areas/get"] = &AsioTcpServer::r_GetActReactCouples;
        m_Handler["/areas/create"] = &AsioTcpServer::r_SetNewARea;
        m_Handler["/areas/remove"] = &AsioTcpServer::r_RemoveActReact;
        m_Handler["/areas/action-update"] = &AsioTcpServer::r_UpdateAction;
        m_Handler["/areas/reaction-update"] = &AsioTcpServer::r_UpdateReaction;
        // --- Subtitles
        m_Handler["/subtitles/get"] = &AsioTcpServer::r_GetSubtitlesSettings;
        m_Handler["/subtitles/set"] = &AsioTcpServer::r_SetSubtitles;
        // --- MTDSIS (Microphone To Display Sources Intelligent Switch)
        m_Handler["/mtdsis/get"] = &AsioTcpServer::r_GetAllLinksMicsToDisplaySources;
        m_Handler["/mtdsis/create"] = &AsioTcpServer::r_LinkMicToDisplaySources;
        m_Handler["/mtdsis/remove"] = &AsioTcpServer::r_UnlinkMicToDisplaySources;
        // --- User Profile (Easystream and OBS parameters)
        m_Handler["/profile/get"] = &AsioTcpServer::r_GetProfileSettings;

        /* OBS Resources routes */
        m_Handler["/scenes/get"] = &AsioTcpServer::r_GetAllScenes;
        m_Handler["/text-fields/get"] = &AsioTcpServer::r_GetAllTextFields;
        m_Handler["/display-sources/get"] = &AsioTcpServer::r_GetAllDisplaySources;

        /* Subscribers / Hooks routes */
        m_Handler["/broadcast/subscribe"] = &AsioTcpServer::r_SubscribeToBroadcast;
    }

    AsioTcpServer::~AsioTcpServer()
    {
    }

    void AsioTcpServer::run(void *)
    {
        blog(LOG_INFO, "###  - Starting server...");
        this->start(); // Initiate server + request handling thread.

        blog(LOG_INFO, "###  - Server started. Now running.");
        // @todo : End thread execution properly
        while (m_PluginManager && m_PluginManager->IsRunning())
        {
            this->update();
            this->thread_sleep_ms(50);
        };
        blog(LOG_INFO, "###  - Server stopped running.");
    }

    void AsioTcpServer::_runRequestHandler(void *private_data)
    {
        while (1)
        {
            while (!this->m_InRequestQueue.empty())
            {
                // Get queue first element.
                const auto &__req_pair = m_InRequestQueue.front();
                Shared<AsioTcpConnection> socket = __req_pair.first; // Get sender socket
                const json &request = __req_pair.second;             // Get request

                // Check if request exists.
                //      @note: May crash if request doesn't contain "command" field.
                //      @todo: Create "try/catch" statement around condition ?
                if (m_Handler.find(request.at("command")) != m_Handler.end())
                {
                    this->_executeRequest(request, socket);
                }
                else
                {
                    m_OutRequestQueue.ts_push(std::make_pair(socket, ResponseGenerator::BadCommand()));
                }

                // Pop out processed element from received requests queue.
                m_InRequestQueue.ts_pop();
            }

            // Sleep 50ms, to save some CPUs lifetime.
            this->thread_sleep_ms(50);
        }
    }

    void AsioTcpServer::_executeRequest(const json &request, Shared<AsioTcpConnection> socket)
    {
        try
        {
            // Call method corresponding to the sent command.
            (this->*m_Handler[request["command"]])(request, socket);
        }
        catch (const json::type_error &type_error)
        {
            // Method .at of json was walled on a non-object
            if (type_error.id == 304)
            {
                m_OutRequestQueue.ts_push(std::make_pair(
                    socket,
                    ResponseGenerator::BadRequest("wrongly formulated.")));
            }
        }
        catch (const json::out_of_range &oor_error)
        {
            // Invalid key was given to the .at method of json (out of range)
            if (oor_error.id == 403)
            {
                m_OutRequestQueue.ts_push(std::make_pair(
                    socket,
                    ResponseGenerator::BadRequest("incomplete - missing value")));
            }
        }
    }

    /***********/
    /* NETWORK */
    /***********/

    bool AsioTcpServer::start()
    {
        try
        {
            _waitForClientConnection();
            this->m_ThreadContext = std::thread(
                [this]()
                {
                    m_IoContext.run();
                });
            blog(LOG_INFO, "### [SERVER EASYSTREAM] new server started on port: %d", m_Acceptor.local_endpoint().port());
            this->m_RequestHandler = std::thread([this]()
                                                 { this->_runRequestHandler(nullptr); });
        }
        catch (std::exception &e)
        {
            blog(LOG_INFO, "### [SERVER EASYSTREAM] Exception: %s", e.what());
            std::cout << "### [SERVER EASYSTREAM] Exception: " << e.what() << std::endl;
            return false;
        }

        return true;
    }

    void AsioTcpServer::_waitForClientConnection()
    {
        this->m_Acceptor.async_accept(
            // Callback on new connection acceptance
            [this](asio::error_code ec, asio::ip::tcp::socket sock)
            {
                if (!ec) // true == New connection success
                {
                    json tmpJson;

                    std::cout << "[SERVER EASYSTREAM] new connection detected " << sock.remote_endpoint() << std::endl;
                    m_Connections.push_back(CreateShared<AsioTcpConnection>(sock));

                    m_Connections.back()->readMessage();
                    tmpJson["socketAddress"] = m_Connections.back()->getSocket().remote_endpoint().address().to_string();
                    tmpJson["socketPort"] = m_Connections.back()->getSocket().remote_endpoint().port();
                    tmpJson["message"] = std::string("successfully connected");
                    tmpJson["statusCode"] = 200;
                    m_Connections.back()->writeMessage(tmpJson.dump());
                }
                else // New connection error
                {
                    std::cout << "[SERVER EASYSTREAM] connection denied: " << ec.message() << std::endl;
                }
                _waitForClientConnection();
            });
    }

    void AsioTcpServer::update()
    {
        // Remove disconnected sockets
        this->m_Connections.erase(
            std::remove_if(
                this->m_Connections.begin(),
                this->m_Connections.end(),
                [this](const Shared<AsioTcpConnection> con)
                {
                    return (!con->isConnected());
                }),
            this->m_Connections.end());

        // Push incoming requests to queue.
        for (auto &con_ : this->m_Connections)
        {
            for (const json &r_ : con_->getMessage())
            {
                m_InRequestQueue.ts_push(std::make_pair(con_, r_));
            }
        }

        // Send messages submitted by request handler

        while (!m_OutRequestQueue.empty())
        {
            const auto &__req_pair = m_OutRequestQueue.front();
            Shared<AsioTcpConnection> socket = __req_pair.first;
            const json &request = __req_pair.second;

            if (socket != nullptr)
            {
                socket->writeMessage(request.dump());
            }

            m_OutRequestQueue.ts_pop();
        }

        while (!m_BroadcastQueue.empty())
        {
            this->_broadcast(m_BroadcastQueue.front());
            m_BroadcastQueue.ts_pop();
        }
    }

    void AsioTcpServer::submitBroadcast(const json &data)
    {
        m_BroadcastQueue.ts_push(data);
    }

    asio::io_context &AsioTcpServer::getContext()
    {
        return this->m_IoContext;
    }

    void AsioTcpServer::_broadcast(const json &data)
    {
        json toSendJson = {
            {"statusCode", 201},
            {"message", "BROADCAST"},
            {"data", data},
        };

        for (auto socket : m_Connections)
        {
            if (socket != nullptr && socket->isSubscribedToBroadcast())
            {
                socket->writeMessage(toSendJson.dump());
            }
        }
    }
}
