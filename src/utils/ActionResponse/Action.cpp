/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** Action
*/

#include "Action.hpp"

es::Action::Action(Response *response)
{
    this->_response = response;
}

es::Action::~Action()
{
    delete this->_response;
}

void es::Action::ActivateResponse() const
{
    if (this->_response)
    {
        this->_response->Resolve();
    }
}

const bool es::Action::IsTrue() const
{
    return this->_isTrue;
}

void es::Action::Reset()
{
    this->_isTrue = false;
}