/*
** EPITECH PROJECT, 2021
** NetworkCommunication
** File description:
** AsioTcpServer
*/

#include "../include/AsioTcpServer.hpp"

namespace es::server
{
    AsioTcpServer::AsioTcpServer(const std::string &host, int port, const std::unordered_map<std::string, std::shared_ptr<obs::AutoAudioLeveler>> &_mps) : _audioLeveler(_mps), /*_endPoint(boost::asio::ip::make_address(host), port),*/ _acceptor(_ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
        /* Getters */
        _handler["getAllMics"] = &AsioTcpServer::getAllMics;
        _handler["getActReactCouples"] = &AsioTcpServer::getActReactCouples;

        /* Setters */
        _handler["setAutoAudioLeveler"] = &AsioTcpServer::setAutoAudioLeveler;
        _handler["setMicLevel"] = &AsioTcpServer::setMicLevel;
        _handler["setSubtitles"] = nullptr;
        _handler["setActionReaction"] = &AsioTcpServer::setNewARea;

        /* Removers */
        _handler["removeActReact"] = &AsioTcpServer::removeActReact;

        /* Updaters */
        _handler["updateAction"] = &AsioTcpServer::updateAction;
        _handler["updateReaction"] = &AsioTcpServer::updateReaction;
    }

    /***********/
    /* NETWORK */
    /***********/

    bool AsioTcpServer::start()
    {
        try
        {
            waitForClientConnection();
            _threadContext = boost::thread([this]()
                                           { _ioContext.run(); });
            blog(LOG_INFO, "### [SERVER EASYSTREAM] new server started on port: %d", _acceptor.local_endpoint().port());
        }
        catch (std::exception &e)
        {
            blog(LOG_INFO, "### [SERVER EASYSTREAM] Exception: %s", e.what());
            return false;
        }

        return true;
    }

    void AsioTcpServer::waitForClientConnection()
    {
        this->_acceptor.async_accept(
            // Callback on new connection acceptance
            [this](boost::system::error_code ec, boost::asio::ip::tcp::socket sock)
            {
                if (!ec) // true == New connection success
                {
                    json tmpJson;

                    std::cout << "[SERVER EASYSTREAM] new connection detected " << sock.remote_endpoint() << std::endl;
                    _connections.push_back(CreateShared<AsioTcpConnection>(sock));

                    _connections.back()->readMessage();
                    tmpJson["socketAdress"] = _connections.back()->getSocket().remote_endpoint().address().to_string();
                    tmpJson["socketPort"] = _connections.back()->getSocket().remote_endpoint().port();
                    tmpJson["Message"] = std::string("succesfully connected");
                    tmpJson["statusCode"] = 200;
                    _connections.back()->writeMessage(tmpJson.dump());
                }
                else // New connection error
                {
                    std::cout << "[SERVER EASYSTREAM] connection denied: " << ec.message() << std::endl;
                }
                waitForClientConnection();
            });
    }

    void AsioTcpServer::writeMessage(const std::string &msg)
    {
        if (_connections.size() > 0)
        {
            _connections[0]->writeMessage(msg);
        }
    }

    void AsioTcpServer::update()
    {
        // Remove disconnected sockets
        this->_connections.erase(
            std::remove_if(
                this->_connections.begin(),
                this->_connections.end(),
                [this](const Shared<AsioTcpConnection> con)
                {
                    return (!con->isConnected());
                }),
            this->_connections.end());

        // Treat requests
        for (auto &con : this->_connections)
        {
            std::vector<json> requests_ = con->getMessage();

            if (requests_.empty()) // @dev (Romain) : Is necessary ?
            {
                continue;
            }
            for (const auto &req : requests_)
            {
                if (_handler.find(req["command"]) != _handler.end())
                {
                    try
                    {
                        // Call method corresponding to the sent command.
                        (this->*_handler[req["command"]])(req, con);
                    }
                    catch (const json::type_error &type_error)
                    {
                        // Method .at of json was walled on a non-object
                        if (type_error.id == 304)
                        {
                            this->badRequest(con, "wrongly formulated.");
                        }
                    }
                    catch (const json::out_of_range &oor_error)
                    {
                        // Invalid key was given to the .at method of json
                        if (oor_error.id == 403)
                        {
                            this->badRequest(con, "incomplete - missing value");
                        }
                    }
                }
                else
                {
                    this->badCommand(con);
                }
            }
        }
    }

    boost::asio::io_context &AsioTcpServer::getContext()
    {
        return this->_ioContext;
    }

    /****************/
    /* GET REQUESTS */
    /****************/

    void AsioTcpServer::getAllMics(const json &j, Shared<AsioTcpConnection> &con)
    {
        json toSend;
        std::vector<json> mics = es::utils::obs::listHelper::GetMicsList();

        for (auto &m : mics)
        {
            float tmpValue = _audioLeveler.find(m["micName"])->second->getDesiredLevel() + 60;

            m["value"] = floor((tmpValue * 100) / 60);
            m["isActive"] = _audioLeveler.find(m["micName"])->second->isActive();
        }

        toSend["statusCode"] = 200;
        toSend["message"] = std::string("OK");
        toSend["length"] = mics.size();
        toSend["mics"] = mics;

        con->writeMessage(toSend.dump());
    }

    void AsioTcpServer::getActReactCouples(const json &j, Shared<AsioTcpConnection> &con)
    {
        json toSend;
        std::vector<json> areas_vec;

        // @todo : retrieve areas from area system and iterate on it.

        for (const auto &pair : this->areas)
        {
            const auto &elem = pair.second;
            json area = {
                {"id", elem.id},
                {"isActive", elem.is_active},
                {"action", {
                               {"actionId", elem.action_data.id},
                               {"type", elem.action_data.type},
                               {"params", elem.action_data.params},
                           }},
                {"reaction", {
                                 {"name", elem.reaction_data.name},
                                 {"reactionId", elem.reaction_data.id},
                                 {"type", elem.reaction_data.type},
                                 {"params", elem.reaction_data.params},
                             }},
            };

            areas_vec.push_back(std::move(area));
        }

        toSend["statusCode"] = 200;
        toSend["message"] = std::string("OK");
        toSend["length"] = this->areas.size();
        toSend["actReacts"] = areas_vec;

        con->writeMessage(toSend.dump());
    }

    /****************/
    /* SET REQUESTS */
    /****************/

    void AsioTcpServer::setAutoAudioLeveler(const json &j, Shared<AsioTcpConnection> &con)
    {
        const json &params = j.at("params");
        const bool &enable = params.at("enable");

        { // @todo : toggle AALeveler for only one audio input
          // auto source = this->_audioLeveler.find(params["source"]);
          // if (source == this->_audioLeveler.end())
          // {
          //     toSend["statusCode"] = 404;
          //     toSend["message"] = std::string("Not found");
          // }
          // else
        }

        if (enable)
        {
            // @todo : globally enable AAL
        }
        else
        {
            // @todo : globally disable AAL
        }

        // Send success response
        const json toSend = {
            {"statusCode", 200},
            {"message", std::string("OK")},
        };
        con->writeMessage(toSend.dump());
    }

    void AsioTcpServer::setMicLevel(const json &j, Shared<AsioTcpConnection> &con)
    {
        const std::string &mic_name = j.at("params").at("name");

        auto mic_iterator = _audioLeveler.find(mic_name);
        // Check if microphone exists
        if (mic_iterator == _audioLeveler.end())
        {
            this->notFound(con, "specified microphone does not exist.");
            return;
        }

        const float &value = j.at("params").at("level");
        const float desired_level = ((value * 60) / 100) - 60; // desired_level -= 60; ??

        // Update microphone level
        mic_iterator->second->setDesiredLevel(desired_level);

        // Send success response
        json toSend = {
            {"statusCode", 200},
            {"message", std::string("OK")},
        };
        con->writeMessage(toSend.dump());
    }

    void AsioTcpServer::setNewARea(const json &j, Shared<AsioTcpConnection> &con)
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

        // Call add function of the ARea system
        // const json &result = area_system.addARea(action, reaction);

        // Send response to client.
        json toSend = {
            {"statusCode", 200},
            {"message", "Act/React couple succesfully created."},
            // {"data", {
            //              {"actReactId", result["id"]},
            //          }},
        };
        con->writeMessage(toSend.dump());
    }

    /*******************/
    /* UPDATE REQUESTS */
    /*******************/

    void AsioTcpServer::updateAction(const json &j, Shared<AsioTcpConnection> &con)
    {
        const json &data = j.at("params");
        area::action_t new_action; // New action data

        // Getting ID of ARea to update
        const size_t area_id = data.at("actionId");

        // Getting new_action type
        new_action.type = ACTION_TYPE_MAP.at(data.at("type"));
        // Getting new_action parameters
        new_action.params = data.at("params");

        // @todo : call func and get result of the updating (Success, Error, Message)
        // const json &result = area_system.updateActionWithId(area_id, new_action)

        // Send success response
        const json toSend = {
            {"statusCode", 200},
            {"message", std::string("OK")},
        };
        con->writeMessage(toSend.dump());
    }

    void AsioTcpServer::updateReaction(const json &j, Shared<AsioTcpConnection> &con)
    {
        const json &data = j.at("params");
        area::reaction_t new_reaction; // New action data

        // Getting ID of ARea to update
        const size_t &area_id = data.at("actionId");

        // Getting new_reaction type
        new_reaction.type = REACTION_TYPE_MAP.at(data.at("type"));
        // Getting new_reaction parameters
        new_reaction.params = data.at("params");

        // @todo : call func and get result of the updating (Success, Error, Message)
        // const json &result = area_system.updateReactionWithId(area_id, new_action)

        // Send success response
        const json toSend = {
            {"statusCode", 200},
            {"message", std::string("OK")},
        };
        con->writeMessage(toSend.dump());
    }

    /*******************/
    /* REMOVE REQUESTS */
    /*******************/

    void AsioTcpServer::removeActReact(const json &j, Shared<AsioTcpConnection> &con)
    {
        json toSend;
        const json params = j.at("params");
        const size_t id_to_rm = params.at("actReactId");

        if (this->areas.find(id_to_rm) == this->areas.end())
        {
            this->notFound(
                con,
                "Provided ID did not match any existing action/reaction couple.");
            return;
        }

        // @todo : Use function to remove ARea
        const area::area_t &to_rm = this->areas[id_to_rm];
        const std::string name_of_removed = to_rm.reaction_data.name;

        // Actually erase area
        this->areas.erase(to_rm.id);

        // Send success message
        toSend["statusCode"] = 200;
        toSend["message"] = "Act/React couple succesfully deleted.";
        toSend["data"] = {
            {"actReactId", id_to_rm},
            {"reaction_name", name_of_removed},
        };

        con->writeMessage(toSend.dump());
    }

    /****************/
    /* BAD REQUESTS */
    /****************/

    void AsioTcpServer::badCommand(Shared<AsioTcpConnection> &con)
    {
        json toSend;

        toSend["statusCode"] = 404;
        toSend["message"] = "The requested action does not exist.";
        con->writeMessage(toSend.dump());
    }

    void AsioTcpServer::badRequest(Shared<AsioTcpConnection> &con, const std::string &msg)
    {
        json toSend;

        toSend["statusCode"] = 400;
        toSend["message"] = std::string("Bad request: ") + msg;
        con->writeMessage(toSend.dump());
    }

    void AsioTcpServer::notFound(Shared<AsioTcpConnection> &con, const std::string &msg)
    {
        json toSend;

        toSend["statusCode"] = 404;
        if (!msg.empty())
        {
            toSend["message"] = std::string("Not found: ") + msg;
        }
        else
        {
            toSend["message"] = std::string("Not found.");
        }
        con->writeMessage(toSend.dump());
    }
}
