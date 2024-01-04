/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** AreaManager
*/

#include "AreaManager.hpp"

namespace es::area
{
    AreaManager::AreaManager()
    {
    }

    AreaManager::~AreaManager()
    {
        this->_actions.clear();
    }

    void AreaManager::run(void *)
    {
        this->thread_sleep_ms(2000);
        blog(LOG_INFO, "###  - AREA system started.");
        while (1)
        {
            this->Update();
            this->thread_sleep_ms(100);
        }
        blog(LOG_INFO, "###  - AREA system has stopped.");
    }

    void AreaManager::Update()
    {
        std::string sentence;
        this->_actions_mutex.lock();
        {
            if (!_words.empty())
            {
                std::lock_guard lock(_mtx);
                sentence = _words.front();
                _words.pop();
            }
        }
        for (auto it : _actions)
        {
            Action *action = it.second;
            auto tmp = action->ToStruct();

            if (tmp.type == area::ActionType::WORD_DETECT)
            {
                static_cast<ActionWordDetect *>(action)->publishTranscription(sentence);
            }
            action->Solve();
            if (action->IsTrue())
            {
                action->ActivateResponse();
            }
            action->Reset();
        }
        // _words.clear();
        this->_actions_mutex.unlock();
    }

    void AreaManager::AddWords(const std::string &w)
    {
        std::unique_lock lock(_mtx);
        std::string wo = w;
        std::transform(wo.begin(), wo.end(), wo.begin(), ::tolower);
        _words.push(wo);
    }

    void AreaManager::AddAction(Action *action)
    {
        this->_actions_mutex.lock();
        this->_actions.insert(std::pair(action->GetId(), action));
        this->_actions_mutex.unlock();
    }

    const bool AreaManager::RemoveAction(const size_t &id)
    {
        this->_actions_mutex.lock();

        Action *action = nullptr;

        // Check if area with id exists
        auto it = this->_actions.find(id);
        if (it == this->_actions.end())
        {
            return false;
        }
        action = this->_actions.extract(it).mapped();
        delete action; // Action::dtor calls delete on reaction.

        this->_actions_mutex.unlock();

        return true;
    }

    const bool AreaManager::AreaExists(const size_t &id)
    {
        if (this->_actions.find(id) == this->_actions.end())
        {
            return false;
        }
        return true;
    }

    /*****************************************/
    /* METHODS CALLED BY THE PLUGIN'S SERVER */
    /*****************************************/

    const std::vector<area::area_t> AreaManager::GetAreas(void)
    {
        std::vector<area::area_t> areas_;

        this->_actions_mutex.lock();
        for (auto it : this->_actions)
        {
            area::area_t area_;
            Action *action_ = it.second;
            Reaction *reaction_ = it.second->GetReaction();

            area_.id = it.first;
            area_.is_active = true;
            area_.action_data = action_->ToStruct();
            area_.reaction_data = reaction_->ToStruct();
            area_.reaction_data.name = reaction_->GetName();

            areas_.push_back(std::move(area_));
        }
        this->_actions_mutex.unlock();

        return areas_;
    }

    const json AreaManager::CreateArea(
        const area::action_t &act_data,
        const area::reaction_t &react_data)
    {
        const size_t area_id = area::ID_COUNTER++;

        Reaction *react = nullptr;
        Action *act = nullptr;

        try
        {
            // Create reaction first
            if (REACTION_TYPE_TO_CREATE_FUNC.at(react_data.type))
            {
                react = REACTION_TYPE_TO_CREATE_FUNC.at(react_data.type)(area_id, react_data.name, react_data.params);
            }
            else
            {
                return {
                    {"return_value", 1}, // 1 = not found
                    {"area_id", area_id},
                    {"message", "Unknown reaction"},
                };
            }

            // Create action and link reaction second
            if (react && ACTION_TYPE_TO_CREATE_FUNC.at(act_data.type))
            {
                act = ACTION_TYPE_TO_CREATE_FUNC.at(act_data.type)(react, area_id, act_data.params);
            }
            else
            {
                return {
                    {"return_value", 1}, // 1 = not found
                    {"area_id", area_id},
                    {"message", "Unknown action"},
                };
            }

            // Add action to AREA system.
            this->AddAction(act);
        }
        catch (std::exception &e)
        {
            const json result = {
                {"return_value", 3}, // 3 = Unknown exception
                {"area_id", area_id},
                {"message", e.what()},
            };
            return result;
        }

        const json result = {
            {"return_value", 0}, // 0 = success
            {"area_id", area_id},
            {"message", "OK"},
        };
        return result;
    }

    const json AreaManager::DeleteArea(const size_t &id)
    {
        auto it = this->_actions.find(id);

        // Check if AREA with provided ID exists
        if (this->RemoveAction(id) == false)
        {
            return json({
                {"return_value", 1},
                {"message", "AREA with provided ID does not exist."},
            });
        }

        return json({
            {"return_value", 0},
            {"message", "AREA successfully deleted"},
            {"area_id", id},
        });
    }

    const json AreaManager::UpdateAction(const size_t &id, const area::action_t &data)
    {
        // Get reaction data from area with id matching argument.
        Action *tmp_act = this->_actions.at(id);
        area::reaction_t react_data = tmp_act->GetReaction()->ToStruct();
        react_data.name = tmp_act->GetReaction()->GetName();

        // Create new area (id will be changed later)
        const json result = this->CreateArea(data, react_data);
        const int ret_val = result.at("return_value");
        if (ret_val != 0)
        {
            return json({
                {"return_value", ret_val},
                {"message", result.at("message")},
            });
        }

        // Delete previous version of area
        this->DeleteArea(id);
        // Extract newly created area from map to change key
        const int tmp_id = result.at("area_id");
        this->_actions_mutex.lock();
        Action *action = this->_actions.extract(tmp_id).mapped();
        this->_actions_mutex.unlock();
        // Set new id to area
        action->SetId(id);
        // Re-add area to map with changed action and original id :)
        this->AddAction(action);

        return json({
            {"return_value", 0}, // 0 = success
            {"message", "action successfully updated"},
            {"area_id", id},
        });
    }

    const json AreaManager::UpdateReaction(const size_t &id, const area::reaction_t &data)
    {
        Reaction *new_react = nullptr;
        Action *action = this->_actions.at(id);

        // Create reaction first
        if (REACTION_TYPE_TO_CREATE_FUNC.at(data.type))
        {
            std::string name = data.name.empty() ? action->GetReaction()->GetName() : data.name;
            new_react = REACTION_TYPE_TO_CREATE_FUNC.at(data.type)(id, name, data.params);
        }
        else
        {
            return {
                {"return_value", 1}, // 1 = not found
                {"area_id", id},
                {"message", "Unknown new reaction type"},
            };
        }

        this->_actions_mutex.lock();
        this->_actions.at(id)->SetReaction(new_react);
        this->_actions_mutex.unlock();

        return json({
            {"return_value", 0}, // 0 = success
            {"message", "reaction successfully updated"},
            {"area_id", id},
        });
    }
}
