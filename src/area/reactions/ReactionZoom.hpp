/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** ReactionZoom
*/

#ifndef REACTIONZOOM_HPP_
#define REACTIONZOOM_HPP_

#include "../Reaction.hpp"

namespace es::area
{
    class ReactionZoom : public Reaction
    {
    public:
        ReactionZoom(const size_t &area_id, const std::string &name, const json &param);
        ~ReactionZoom();

        virtual void Resolve() override;
        virtual es::area::reaction_t ToStruct() override;
    protected:
    private:
        struct vec2 ZoomRatio = {1, 1};
    };
}

#endif /* !REACTIONZOOM_HPP_ */
