/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** ReactionRecordingStop
*/

#ifndef REACTIONRECORDINGSTOP_HPP_
#define REACTIONRECORDINGSTOP_HPP_

#include "../Reaction.hpp"

namespace es::area
{
    class ReactionRecordingStop : public Reaction
    {
    public:
        ReactionRecordingStop(const size_t &area_id, const std::string &name, const json &param);
        ~ReactionRecordingStop();

        virtual void Resolve() override;
        virtual es::area::reaction_t ToStruct() override;

    protected:
    private:
        uint _delay = 0;
    };
}

#endif /* !REACTIONRECORDINGSTOP_HPP_ */
