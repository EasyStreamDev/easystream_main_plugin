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
        /* Getters */
        m_Handler["getAllMics"] = &AsioTcpServer::r_GetAllMics;
        m_Handler["getActReactCouples"] = &AsioTcpServer::r_GetActReactCouples;

        /* Setters */
        m_Handler["setAutoAudioLeveler"] = &AsioTcpServer::r_SetAutoAudioLeveler;
        m_Handler["setMicLevel"] = &AsioTcpServer::r_SetMicLevel;
        m_Handler["setSubtitles"] = nullptr;
        m_Handler["setActionReaction"] = &AsioTcpServer::r_SetNewARea;

        /* Removers */
        m_Handler["removeActReact"] = &AsioTcpServer::r_RemoveActReact;

        /* Updaters */
        m_Handler["updateAction"] = &AsioTcpServer::r_UpdateAction;
        m_Handler["updateReaction"] = &AsioTcpServer::r_UpdateReaction;
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
                    this->_createRequestExecutorThread(request, socket);
                }
                else
                {
                    m_OutRequestQueue.ts_push(std::make_pair(socket, ResponseGenerator::BadCommand()));
                }

                // Pop out processed element from received requests queue.
                m_InRequestQueue.ts_pop();
            }

            // Sleep 50ms to not run fullspeed.
            this->thread_sleep_ms(50);
        }
    }

    void AsioTcpServer::_createRequestExecutorThread(const json &request, Shared<AsioTcpConnection> socket)
    {
        // Start new thread to execute request
        std::thread w(
            [this, request, socket]()
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
                    // Invalid key was given to the .at method of json
                    if (oor_error.id == 403)
                    {
                        m_OutRequestQueue.ts_push(std::make_pair(
                            socket,
                            ResponseGenerator::BadRequest("incomplete - missing value")));
                    }
                }
            });
        w.detach();
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
            // std::cout << "### [SERVER EASYSTREAM] new server started on " << m_Acceptor.local_endpoint().address() << ":" << m_Acceptor.local_endpoint().port() << std::endl;
            // Start request handler
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
                    tmpJson["socketAdress"] = m_Connections.back()->getSocket().remote_endpoint().address().to_string();
                    tmpJson["socketPort"] = m_Connections.back()->getSocket().remote_endpoint().port();
                    tmpJson["Message"] = std::string("succesfully connected");
                    tmpJson["statusCode"] = 200;
                    m_Connections.back()->writeMessage(tmpJson.dump() + "\r\n");
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

        std::cout << "Broadcasting following message :" << std::endl;
        std::cout << toSendJson << std::endl;

        for (auto socket : m_Connections)
        {
            if (socket != nullptr)
            {
                socket->writeMessage(toSendJson.dump() + "\r\n");
            }
        }
    }
}
