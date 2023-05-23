/*
** NetworkCommunication
** File description:
** ServerRequests
*/

#include "../include/AsioTcpServer.hpp"

namespace es::server
{
    /****************/
    /* GET REQUESTS */
    /****************/

    void AsioTcpServer::r_GetAllMics(const json &j, Shared<AsioTcpConnection> con)
    {
        const json response_data = get_mics_data(m_PluginManager->GetSourceTracker());

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

    void AsioTcpServer::r_SetCompressorLevel(const json &j, Shared<AsioTcpConnection> con)
    {
        const json &params = j.at("params");
        const std::string &source_name = params.at("micName");
        const bool &enable = params.at("isActive");

        auto autoLevelerMap_ = m_PluginManager->GetSourceTracker()->getAudioMap();
        auto source_target = autoLevelerMap_.find(source_name);

        // Check if microphone exists
        if (source_target == autoLevelerMap_.end())
        {
            m_OutRequestQueue.ts_push(std::make_pair(
                con,
                ResponseGenerator::NotFound("Specified audio source does not exist: " + source_name)));
            return;
        }

        const float &inv_value = 100.0 - (float)params.at("level");
        const float desired_level = ((inv_value * 60) / 100) - 60; // desired_level -= 60; ??

        // Update microphone level
        source_target->second->setDesiredLevel(desired_level);
        source_target->second->SetActive(enable);

        // Send success response to asking client.
        m_OutRequestQueue.ts_push(std::make_pair(con, ResponseGenerator::Success()));

        // Broadcast new mics levels to all client.
        json broadcast_request = get_mics_data(m_PluginManager->GetSourceTracker());
        broadcast_request["type"] = "micLevelChanged";
        this->submitBroadcast(broadcast_request);
    }

    void AsioTcpServer::r_SetNewARea(const json &j, Shared<AsioTcpConnection> con)
    {
        area::action_t action;
        area::reaction_t reaction;
        std::cout << "======================================error here 1====================================" << std::endl;
        const json action_data = j.at("params").at("action");
        const json reaction_data = j.at("params").at("reaction");

        // Setting up action_t struct
        action.type = ACTION_TYPE_MAP.at(action_data.at("type"));
        std::cout << "======================================error here 2 ====================================" << std::endl;
        action.params = action_data.at("params");
        // Setting up reaction_t struct
        std::cout << "======================================error here 3====================================" << std::endl;
        reaction.name = reaction_data.at("name");
        std::cout << "======================================error here 4====================================" << std::endl;
        reaction.type = REACTION_TYPE_MAP.at(reaction_data.at("type"));
        std::cout << "======================================error here 5====================================" << std::endl;
        reaction.params = reaction_data.at("params");

        // Create AREA within AREA system.
        const json result = m_PluginManager->GetAreaMain()->CreateArea(action, reaction);
        std::cout << "======================================error here 6====================================" << std::endl;
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

    void AsioTcpServer::r_SetSubtitles(const json &j, Shared<AsioTcpConnection> con)
    {
        const json params = j.at("params");

        m_OutRequestQueue.ts_push(std::make_pair(
            con,
            ResponseGenerator::Success("OK")));
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
        m_OutRequestQueue.ts_push(
            std::make_pair(
                con,
                ResponseGenerator::Success(msg, json({{"actReactId", result.at("area_id")}}))));
    }

    /*************************/
    /* SUBSCRIPTION REQUESTS */
    /*************************/

    void AsioTcpServer::r_SubscribeToBroadcast(const json &j, Shared<AsioTcpConnection> con)
    {
        const json params = j.at("params");
        const bool value = params.at("enable");

        con->setBroadcastSubscription(value);

        m_OutRequestQueue.ts_push(std::make_pair(con, ResponseGenerator::Success()));
    }

    /********************/
    /* USEFUL FUNCTIONS */
    /********************/

    const json get_mics_data(es::obs::SourceTracker *source_tracker)
    {
        json toSend;
        std::vector<json> mics = es::utils::obs::listHelper::GetMicsList();

        auto autoLevelerMap_ = source_tracker->getAudioMap();

        for (auto &m : mics)
        {
            std::shared_ptr<es::obs::AutoAudioLeveler> micAudioLeveler_ = autoLevelerMap_.find(m["micName"])->second;

            float tmpValue = micAudioLeveler_->getDesiredLevel() + 60;

            m["level"] = 100.0 - floor((tmpValue * 100) / 60);
            m["isActive"] = micAudioLeveler_->isActive();
        }

        return {
            {"length", mics.size()},
            {"mics", mics},
        };
    }
}