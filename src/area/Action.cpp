/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** Action
*/

#include "Action.hpp"

namespace es::area
{
    Action::Action(Reaction *reaction, const size_t &area_id, const json &param)
        : _id(area_id)
    {
        this->_reaction = reaction;
    }

    Action::~Action()
    {
        delete this->_reaction;
    }

    void Action::ActivateResponse() const
    {
        if (this->_reaction)
        {
            this->_reaction->Resolve();
        }
    }

    const bool Action::IsTrue() const
    {
        return this->_isTrue;
    }

    void Action::Reset()
    {
        this->_isTrue = false;
    }

    void Action::SetReaction(Reaction *new_reaction_)
    {
        delete this->_reaction;
        this->_reaction = new_reaction_;
    }
}