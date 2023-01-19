/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** Action
*/

#include "Action.hpp"

es::Action::Action(Response *response)
{
    _Response = response;
}

es::Action::~Action()
{
}

void es::Action::ActivateResponse() const
{
    if (_Response)
        _Response->Resolve();
}

bool es::Action::IsTrue() const
{
    return _IsTrue;
}

void es::Action::Reset()
{
    _IsTrue = false;
}