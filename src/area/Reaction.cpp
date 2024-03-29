/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** Reaction
*/

#include "Reaction.hpp"

namespace es::area
{
    Reaction::Reaction(const size_t &area_id, const std::string &name, const json &param)
        : _id(area_id), _name(name)
    {
    }
}