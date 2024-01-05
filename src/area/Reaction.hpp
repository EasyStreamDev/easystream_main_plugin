/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** Reaction
*/

#ifndef REACTION_HPP_
#define REACTION_HPP_

#include "../Common.hpp"
#include "AreaException.hpp"

namespace es
{
    class IPluginManager;
}

namespace es::area
{
    class Reaction
    {
    public:
        Reaction(const size_t &, const std::string &, const json &param);
        ~Reaction() = default;

        virtual void Resolve() = 0;

        virtual es::area::reaction_t ToStruct() = 0;

        inline const std::string &GetName(void) const { return _name; };

        es::IPluginManager *_pm;

    protected:
        const size_t _id;
        const std::string _name;
    };
}

#endif /* !REACTION_HPP_ */
