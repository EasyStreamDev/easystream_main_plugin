/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ActionReactionMain
*/

#include "ActionReactionMain.hpp"

namespace es
{
    ActionReactionMain::ActionReactionMain()
    {
    }

    ActionReactionMain::~ActionReactionMain()
    {
        this->_actions_mutex.lock();
        for (auto it : this->_actions)
        {
            delete it.second;
        }
        this->_actions.clear();
        this->_actions_mutex.unlock();
    }

    void ActionReactionMain::run()
    {
        while (1)
        {
            this->Update();
        }
    }

    void ActionReactionMain::Update()
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        this->_actions_mutex.lock();
        for (auto it : _actions)
        {
            Action *action = it.second;

            action->Solve();
            if (action->IsTrue())
            {
                action->ActivateResponse();
            }
            action->Reset();
        }
        this->_actions_mutex.unlock();
    }

    void ActionReactionMain::AddAction(Action *action)
    {
        this->_actions_mutex.lock();
        this->_actions.insert(std::pair(action->GetId(), action));
        this->_actions_mutex.unlock();
    }

    const bool ActionReactionMain::RemoveAction(const size_t &id)
    {
        this->_actions_mutex.lock();
        const int res = this->_actions.erase(id); // @note : erase of ptr does not free memory
        this->_actions_mutex.unlock();

        return (res == 1) ? true : false;
    }

    /*****************************************/
    /* METHODS CALLED BY THE PLUGIN'S SERVER */
    /*****************************************/

    const std::unordered_map<size_t, area::area_t> ActionReactionMain::GetAreas(void) const
    {
    }

    const json ActionReactionMain::CreateArea(
        const area::action_t &act_data,
        const area::reaction_t &react_data)
    {
        const size_t area_id = area::ID_COUNTER++;

        Reaction *react = nullptr;
        Action *act = nullptr;

        if (REACTION_TYPE_TO_CREATE_FUNC.at(react_data.type))
        {
            react = REACTION_TYPE_TO_CREATE_FUNC.at(react_data.type)(area_id, react_data.params);
        }
        else
            return {
                {"return_value", 1}, // 1 = not found
                {"area_id", area_id},
                {"message", "Unknown reaction"},
            };
        if (react && ACTION_TYPE_TO_CREATE_FUNC.at(act_data.type))
        {
            act = ACTION_TYPE_TO_CREATE_FUNC.at(act_data.type)(react, area_id, act_data.params);
        }
        else
            return {
                {"return_value", 1}, // 1 = not found
                {"area_id", area_id},
                {"message", "Unknown action"},
            };

        this->AddAction(act);

        const json result = {
            {"return_value", 0}, // 0 = success
            {"area_id", area_id},
            {"message", "OK"},
        };
        return result;
    }

    const json ActionReactionMain::DeleteArea(const size_t &id)
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

    const json ActionReactionMain::UpdateAction(const size_t &id, const area::action_t &data)
    {
        // @todo: Update action of area with id matching
        return json({
            {"return_value", 0}, // 0 = success
            {"message", "action successfully updated"},
        });
    }

    const json ActionReactionMain::UpdateReaction(const size_t &id, const area::reaction_t &data)
    {
        // @todo: Update action of area with id matching
        return json({
            {"return_value", 0}, // 0 = success
            {"message", "reaction successfully updated"},
        });
    }
}
