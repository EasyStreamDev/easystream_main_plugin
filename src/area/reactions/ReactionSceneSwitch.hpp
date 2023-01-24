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
    class ReactionSceneSwitch : public Reaction
    {
    public:
        ReactionSceneSwitch(const size_t &area_id, const json &param);
        ~ReactionSceneSwitch();

        virtual void Resolve() override;
        virtual es::area::reaction_t ToStruct() override;

    protected:
    private:
        std::string _sceneToSwitch = "";
    };
}

#endif /* !REACTIONSCENESWITCH_HPP_ */
