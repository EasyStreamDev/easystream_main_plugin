/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** ReactionRecordingStart
*/

#ifndef REACTIONRECORDINGSTART_HPP_
#define REACTIONRECORDINGSTART_HPP_

#include "../Reaction.hpp"

namespace es::area
{
    class ReactionRecordingStart : public Reaction
    {
    public:
        ReactionRecordingStart(const size_t &area_id, const std::string &name, const json &param);
        ~ReactionRecordingStart();

        virtual void Resolve() override;
        virtual es::area::reaction_t ToStruct() override;

    protected:
    private:
        uint _delay = 0;
    };
}

#endif /* !REACTIONRECORDINGSTART_HPP_ */
