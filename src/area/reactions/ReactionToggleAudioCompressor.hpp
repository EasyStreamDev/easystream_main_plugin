/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ReactionToggleAudioCompressor
*/

#ifndef REACTIONTOGGLEAUDIOCOMPRESSOR_HPP_
#define REACTIONTOGGLEAUDIOCOMPRESSOR_HPP_

#include "../Reaction.hpp"

namespace es::area
{
    class ReactionToggleAudioCompressor : public Reaction
    {
    public:
        ReactionToggleAudioCompressor(const size_t &, const std::string &, const json &);
        ~ReactionToggleAudioCompressor();

        virtual void Resolve() override;
        virtual es::area::reaction_t ToStruct() override;

    protected:
    private:
        bool _toggle;
        std::string _uuid;
    };
}

#endif /* !REACTIONTOGGLEAUDIOCOMPRESSOR_HPP_ */
