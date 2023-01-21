/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** TestAction
*/

#include "TestAction.hpp"

es::TestAction::TestAction(Reaction *reaction, const size_t &area_id)
    : Action(reaction, area_id)
{
}

es::TestAction::~TestAction()
{
}

void es::TestAction::Solve()
{
    this->_isTrue = true;
}