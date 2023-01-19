/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** TestAction
*/

#include "TestAction.hpp"

es::TestAction::TestAction(Response *response)
    : Action(response)
{
}

es::TestAction::~TestAction()
{
}

void es::TestAction::Solve()
{
    _IsTrue = true;
}