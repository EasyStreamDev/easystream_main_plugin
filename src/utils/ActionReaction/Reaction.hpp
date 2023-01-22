/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** Reaction
*/

#ifndef REACTION_HPP_
#define REACTION_HPP_

#include "../../Common.hpp"

namespace es
{
    class Reaction
    {
    public:
        Reaction(const size_t &, const json &param);
        ~Reaction() = default;

        virtual void Resolve() = 0;

        virtual es::area::reaction_t ToStruct() = 0;

    protected:
        const size_t _id;
    };
}

#endif /* !REACTION_HPP_ */
