/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ActionKeyPressed
*/

#include "ActionKeyPressed.hpp"

namespace es::area
{
    ActionKeyPressed::ActionKeyPressed(Reaction *reaction, const size_t &area_id, const json &param)
        : Action(reaction, area_id, param)
    {
        _key = param["key"].get<std::string>();
    }

    ActionKeyPressed::~ActionKeyPressed()
    {
    }

    void ActionKeyPressed::Solve()
    {
        this->_isTrue = true;
    }

    action_t ActionKeyPressed::ToStruct()
    {
        return {
            _id,
            ActionType::KEY_PRESSED,
            {{"key", _key}}};
    }
}
