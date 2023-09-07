/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** ReactionMove
*/

#ifndef REACTIONMOVE_HPP_
#define REACTIONMOVE_HPP_

#include "../Reaction.hpp"

namespace es::area
{
    // UP = 0,
    // DOWN = 1,
    // LEFT = 2,
    // RIGHT = 3,

    class ReactionMove : public Reaction
    {
    public:
        ReactionMove(const size_t &area_id, const std::string &name, const json &param);
        ~ReactionMove();

        virtual void Resolve() override;
        virtual es::area::reaction_t ToStruct() override;
    protected:
    private:
        float TranslationIncrease = 0;
        int Direction = 0;
    };
}

#endif /* !REACTIONMOVE_HPP_ */
