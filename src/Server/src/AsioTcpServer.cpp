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
        _handler["updateAction"] = nullptr;
        _handler["updateReaction"] = nullptr;
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
                    (this->*_handler[req["command"]])(req, con);
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

        for (const auto &pair : this->areas)
        {
            const auto &elem = pair.second;
            json area = {
                // {"name", elem.name},
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
        json toSend;
        json args_ = j["args"];
        bool enable_ = args_["enable"];
        auto source_ = this->_audioLeveler.find(args_["source"]);

        if (source_ == this->_audioLeveler.end())
        {
            toSend["statusCode"] = 404;
            toSend["message"] = std::string("Not found");
        }
        else if (enable_)
        {
            // enable
        }
        else
        {
            // disable
        }
        con->writeMessage(toSend.dump());
    }

    void AsioTcpServer::setMicLevel(const json &j, Shared<AsioTcpConnection> &con)
    {
        json toSend;
        float value = j["args"]["value"];
        auto tmp = _audioLeveler.find(j["args"]["micId"]);

        if (tmp == _audioLeveler.end())
        {
            toSend["statusCode"] = 404;
            toSend["message"] = std::string("Not found");
        }
        else
        {
            float level = ((value * 60) / 100) - 60;
            // level -= 60;
            // std::cout << "level is: " << level << std::endl;
            tmp->second->setDesiredLevel(level);
            toSend["statusCode"] = 200;
            toSend["message"] = std::string("OK");
        }
        con->writeMessage(toSend.dump());
    }

    void AsioTcpServer::setNewARea(const json &j, Shared<AsioTcpConnection> &con)
    {
        area::action_t action;
        area::reaction_t reaction;
        json result;

        try
        {
            const json action_data = j["params"]["action"];
            const json reaction_data = j["params"]["reaction"];

            // Setting up action_t struct
            action.type = ACTION_TYPE_MAP.at(action_data["type"]);
            action.params = action_data["params"];

            // Setting up reaction_t struct
            reaction.name = reaction_data["name"];
            reaction.type = REACTION_TYPE_MAP.at(reaction_data["type"]);
            reaction.params = reaction_data["params"];
        }
        catch (const std::exception &e)
        {
            // Send error to client.
        }

        // Call add function of the ARea system
        // result = area_system.addARea(action, reaction);

        // Send result to client.
    }

    /*******************/
    /* UPDATE REQUESTS */
    /*******************/

    void AsioTcpServer::updateAction(const json &j, Shared<AsioTcpConnection> &con)
    {
        size_t area_id;            // Area ID to modify
        area::action_t new_action; // New action data
        json result;               // Result of the updating (Success, Error, Message)

        try
        {
            const json data = j["params"];

            // Getting ARea ID
            area_id = data["actionId"];

            // Getting new_action type
            new_action.type = ACTION_TYPE_MAP.at(data["type"]);
            // Getting new_action parameters
            new_action.params = data["params"];
        }
        catch (const std::exception &e)
        {
            // Invalid request data
        }
        // result = area_system.updateActionWithId(area_id, new_action)
    }

    /*******************/
    /* REMOVE REQUESTS */
    /*******************/

    void AsioTcpServer::removeActReact(const json &j, Shared<AsioTcpConnection> &con)
    {
        json toSend;
        json params = j["params"];
        size_t id_to_rm = params["actReactId"];

        if (this->areas.find(id_to_rm) == this->areas.end())
        {
            toSend["statusCode"] = 404;
            toSend["message"] = std::string("Provided ID did not match any created action/reaction couple.");
            con->writeMessage(toSend.dump());
            return;
        }

        area::area_t &to_rm = this->areas[id_to_rm];

        toSend["statusCode"] = 200;
        toSend["message"] = "Act/React couple succesfully deleted.";
        toSend["data"] = {
            {"reaction_name", to_rm.reaction_data.name},
            {"actReactId", to_rm.id},
        };
        this->areas.erase(to_rm.id);

        con->writeMessage(toSend.dump());
    }

    /****************/
    /* BAD REQUESTS */
    /****************/

    void AsioTcpServer::badCommand(Shared<AsioTcpConnection> &con)
    {
        json toSend;

        toSend["statusCode"] = 404;
        toSend["message"] = "The requested action does not exist";
        con->writeMessage(toSend.dump());
    }
}
