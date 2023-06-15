/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ReactionSceneSwitch
*/

#ifndef REACTIONSCENESWITCH_HPP_
#define REACTIONSCENESWITCH_HPP_

#include "../Reaction.hpp"

namespace es::area
{
    struct scene_data
    {
        std::string name;
        std::string uuid;
    };

    class ReactionSceneSwitch : public Reaction
    {
    public:
        ReactionSceneSwitch(const size_t &, const std::string &, const json &);
        ~ReactionSceneSwitch();

        virtual void Resolve() override;
        virtual es::area::reaction_t ToStruct() override;

    protected:
    private:
        scene_data _sceneToSwitch;
    };
}

#endif /* !REACTIONSCENESWITCH_HPP_ */
