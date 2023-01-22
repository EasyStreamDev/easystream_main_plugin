/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** Action
*/

#include "Action.hpp"

es::Action::Action(Reaction *reaction, const size_t &area_id, const json &param)
    : _id(area_id)
{
    this->_reaction = reaction;
}

es::Action::~Action()
{
    delete this->_reaction;
}

void es::Action::ActivateResponse() const
{
    if (this->_reaction)
    {
        this->_reaction->Resolve();
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