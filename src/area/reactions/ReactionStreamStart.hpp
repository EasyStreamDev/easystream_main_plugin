/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** ReactionStreamStart
*/

#ifndef REACTIONSTREAMSTART_HPP_
#define REACTIONSTREAMSTART_HPP_

#include "../Reaction.hpp"

namespace es::area
{
    class ReactionStreamStart : public Reaction
    {
    public:
        ReactionStreamStart(const size_t &area_id, const std::string &name, const json &param);
        ~ReactionStreamStart();

        virtual void Resolve() override;
        virtual es::area::reaction_t ToStruct() override;

    protected:
    private:
        uint _delay = 0;
    };
}

#endif /* !REACTIONSTREAMSTART_HPP_ */
