/*
** EPITECH PROJECT, 2021
** NetworkCommunication
** File description:
** AsioTcpServer
*/

#include "../include/AsioTcpServer.hpp"
#include "../../obs/speechRecognition/transcript/TranscriptorManager.hpp"

// using namespace std::chrono_literals;

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
    }

    asio::io_context &AsioTcpServer::getContext()
    {
        return this->m_IoContext;
    }

    /****************/
    /* GET REQUESTS */
    /****************/

    void AsioTcpServer::r_GetAllMics(const json &j, Shared<AsioTcpConnection> con)
    {
        json toSend;
        std::vector<json> mics = es::utils::obs::listHelper::GetMicsList();

        auto autoLevelerMap_ = m_PluginManager->GetSourceTracker()->getAudioMap();

        for (auto &m : mics)
        {
            std::shared_ptr<es::obs::AutoAudioLeveler> micAudioLeveler_ = autoLevelerMap_.find(m["micName"])->second;

            float tmpValue = micAudioLeveler_->getDesiredLevel() + 60;

            m["level"] = floor((tmpValue * 100) / 60);
            m["isActive"] = micAudioLeveler_->isActive();
        }

        const json response_data = {
            {"length", mics.size()},
            {"mics", mics},
        };
        // Submit response to outgoing requests queue.
        m_OutRequestQueue.ts_push(std::make_pair(con, ResponseGenerator::Success("OK", response_data)));
    }

    void AsioTcpServer::r_GetActReactCouples(const json &j, Shared<AsioTcpConnection> con)
    {
        std::vector<json> areas_vec;

        for (const auto &area : m_PluginManager->GetAreaMain()->GetAreas())
        {
            json area_data = {
                {"actReactId", area.id},
                {"isActive", area.is_active},
                {"action", {
                               //  {"actionId", area.action_data.id},
                               {"type", area::ActionTypeToString(area.action_data.type)},
                               {"params", area.action_data.params},
                           }},
                {"reaction", {
                                 //  {"reactionId", area.reaction_data.id},
                                 {"name", area.reaction_data.name},
                                 {"type", area::ReactionTypeToString(area.reaction_data.type)},
                                 {"params", area.reaction_data.params},
                             }},
            };
            areas_vec.push_back(std::move(area_data));
        }

        const json response_data = {
            {"length", areas_vec.size()},
            {"actReacts", areas_vec},
        };
        // Submit response to outgoing requests queue.
        m_OutRequestQueue.ts_push(std::make_pair(con, ResponseGenerator::Success("OK", response_data)));
    }

    /****************/
    /* SET REQUESTS */
    /****************/

    void AsioTcpServer::r_SetAutoAudioLeveler(const json &j, Shared<AsioTcpConnection> con)
    {
        const json &params = j.at("params");
        const bool &enable = params.at("enable");
        const std::string &source_name = params.at("source");
        auto autoLevelerMap_ = m_PluginManager->GetSourceTracker()->getAudioMap();

        auto source_target = autoLevelerMap_.find(source_name);
        if (source_target == autoLevelerMap_.end())
        {
            m_OutRequestQueue.ts_push(std::make_pair(
                con,
                ResponseGenerator::NotFound(std::string("Source not found : ") + source_name)));
        }

        source_target->second->SetActive(enable);
        // Submit response to outgoing requests queue.
        m_OutRequestQueue.ts_push(std::make_pair(con, ResponseGenerator::Success()));
    }

    void AsioTcpServer::r_SetMicLevel(const json &j, Shared<AsioTcpConnection> con)
    {
        const std::string &mic_name = j.at("params").at("micName");
        auto autoLevelerMap_ = m_PluginManager->GetSourceTracker()->getAudioMap();
        auto it_MicAutoLeveler_ = autoLevelerMap_.find(mic_name);

        // Check if microphone exists
        if (it_MicAutoLeveler_ == autoLevelerMap_.end())
        {
            m_OutRequestQueue.ts_push(std::make_pair(
                con,
                ResponseGenerator::NotFound("specified microphone does not exist.")));
            return;
        }

        const float &value = j.at("params").at("level");
        const float desired_level = ((value * 60) / 100) - 60; // desired_level -= 60; ??

        // Update microphone level
        it_MicAutoLeveler_->second->setDesiredLevel(desired_level);

        // Send success response
        m_OutRequestQueue.ts_push(std::make_pair(con, ResponseGenerator::Success()));
    }

    void AsioTcpServer::r_SetNewARea(const json &j, Shared<AsioTcpConnection> con)
    {
        area::action_t action;
        area::reaction_t reaction;
        const json action_data = j.at("params").at("action");
        const json reaction_data = j.at("params").at("reaction");

        // Setting up action_t struct
        action.type = ACTION_TYPE_MAP.at(action_data.at("type"));
        action.params = action_data.at("params");
        // Setting up reaction_t struct
        reaction.name = reaction_data.at("name");
        reaction.type = REACTION_TYPE_MAP.at(reaction_data.at("type"));
        reaction.params = reaction_data.at("params");

        // Create AREA within AREA system.
        const json result = m_PluginManager->GetAreaMain()->CreateArea(action, reaction);

        // Error on creation of AREA
        if (result.at("return_value") != 0)
        {
            m_OutRequestQueue.ts_push(std::make_pair(con, ResponseGenerator::BadRequest(result.at("message"))));
            return;
        }

        // AREA successfully created
        const json response_data = {
            {"actReactId", result.at("area_id")},
        };

        m_OutRequestQueue.ts_push(std::make_pair(
            con,
            ResponseGenerator::Success(
                "Act/React couple succesfully created.",
                response_data)));
    }

    /*******************/
    /* UPDATE REQUESTS */
    /*******************/

    void AsioTcpServer::r_UpdateAction(const json &j, Shared<AsioTcpConnection> con)
    {
        const json &data = j.at("params");
        area::action_t new_action; // New action data
        // Getting ID of ARea to update
        const size_t area_id = data.at("actionId");

        // Getting new_action type
        new_action.type = ACTION_TYPE_MAP.at(data.at("type"));
        // Getting new_action parameters
        new_action.params = data.at("params");

        // Call to updating function
        const json result = m_PluginManager->GetAreaMain()->UpdateAction(area_id, new_action);
        const int &ret_val = result.at("return_value");
        const std::string msg = result.at("message");

        if (ret_val != 0)
        {
            m_OutRequestQueue.ts_push(std::make_pair(con, ResponseGenerator::NotFound(msg)));
            return;
        }

        m_OutRequestQueue.ts_push(std::make_pair(
            con,
            ResponseGenerator::Success(
                msg,
                json({
                    {"actReactId", result.at("area_id")},
                }))));
    }

    void AsioTcpServer::r_UpdateReaction(const json &j, Shared<AsioTcpConnection> con)
    {
        const json &data = j.at("params");
        // Getting ID of ARea to update
        const size_t &area_id = data.at("reactionId");
        // New action data
        area::reaction_t new_reaction;

        if (data.contains("name"))
        {
            new_reaction.name = data.at("name");
        }
        // Getting new_reaction type
        new_reaction.type = REACTION_TYPE_MAP.at(data.at("type"));
        // Getting new_reaction parameters
        new_reaction.params = data.at("params");

        // Call to updating function
        const json result = m_PluginManager->GetAreaMain()->UpdateReaction(area_id, new_reaction);
        const int &ret_val = result.at("return_value");
        const std::string msg = result.at("message");

        if (ret_val != 0)
        {
            m_OutRequestQueue.ts_push(std::make_pair(con, ResponseGenerator::NotFound(msg)));
            return;
        }

        m_OutRequestQueue.ts_push(std::make_pair(
            con,
            ResponseGenerator::Success(
                msg,
                json({
                    {"actReactId", result.at("area_id")},
                }))));
    }

    /*******************/
    /* REMOVE REQUESTS */
    /*******************/

    void AsioTcpServer::r_RemoveActReact(const json &j, Shared<AsioTcpConnection> con)
    {
        const json params = j.at("params");
        const size_t id_to_rm = params.at("actReactId");

        // Call delete AREA function
        const json result = m_PluginManager->GetAreaMain()->DeleteArea(id_to_rm);
        const int &ret_val = result.at("return_value");
        const std::string msg = result.at("message");

        // Error on AREA deletion
        if (ret_val != 0)
        {
            m_OutRequestQueue.ts_push(std::make_pair(con, ResponseGenerator::NotFound(msg)));
        }

        // Send success message
        m_OutRequestQueue.ts_push(std::make_pair(
            con,
            ResponseGenerator::Success(
                msg,
                json({
                    {"actReactId", result.at("area_id")},
                }))));
    }
}
