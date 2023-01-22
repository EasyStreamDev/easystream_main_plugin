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
    // _key = param["Key"].get<std::string>();
}

es::ActionKeyPressed::~ActionKeyPressed()
{
}

void es::ActionKeyPressed::Solve()
{
    this->_isTrue = true;
}