/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** ReactionStreamStop
*/

#ifndef REACTIONSTREAMSTOP_HPP_
#define REACTIONSTREAMSTOP_HPP_

#include "../Reaction.hpp"

namespace es::area
{
    class ReactionStreamStop : public Reaction
    {
    public:
        ReactionStreamStop(const size_t &area_id, const std::string &name, const json &param);
        ~ReactionStreamStop();

        virtual void Resolve() override;
        virtual es::area::reaction_t ToStruct() override;

    protected:
    private:
        uint _delay = 0;
    };
}

#endif /* !REACTIONSTREAMSTOP_HPP_ */
