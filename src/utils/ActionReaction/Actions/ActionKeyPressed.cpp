/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ActionKeyPressed
*/

#include "ActionKeyPressed.hpp"

es::ActionKeyPressed::ActionKeyPressed(Reaction *reaction, const size_t &area_id, const json &param)
    : Action(reaction, area_id, param)
{
    _key = param["key"].get<std::string>();
}

es::ActionKeyPressed::~ActionKeyPressed()
{
}

void es::ActionKeyPressed::Solve()
{
    this->_isTrue = true;
}

es::area::action_t es::ActionKeyPressed::ToStruct()
{
    return {
        _id,
        es::area::ActionType::KEY_PRESSED,
        {{"key", _key}}
    };
}