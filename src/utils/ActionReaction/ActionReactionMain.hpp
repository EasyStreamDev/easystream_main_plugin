/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ActionReactionMain
*/

#ifndef ACTIONREACTIONMAIN_HPP_
#define ACTIONREACTIONMAIN_HPP_

#include "../../Common.hpp"

// Actions includes
#include "Actions/TestAction.hpp"

// Reaction includes
#include "Reactions/TestReaction.hpp"

namespace es
{
    class ActionReactionMain
    {
    public:
        // @todo: set functions to create actions / reactions
        const std::unordered_map<area::ActionType, Action *(*)(const json &)>
            ACTION_TYPE_TO_CREATE_FUNC = {
                {area::ActionType::KEY_PRESSED, nullptr},
                {area::ActionType::APP_LAUNCH, nullptr},
                {area::ActionType::WORD_DETECT, nullptr},
        };
        const std::unordered_map<area::ReactionType, Reaction *(*)(const json &)>
            REACTION_TYPE_TO_CREATE_FUNC = {
                {area::ReactionType::SCENE_SWITCH, nullptr},
                {area::ReactionType::TOGGLE_AUDIO_COMPRESSOR, nullptr},
        };

    public:
        ActionReactionMain();
        ~ActionReactionMain();

        void run();

        void Update();
        void AddAction(Action *action);
        const bool RemoveAction(const size_t &id);

        /*****************************************/
        /* METHODS CALLED BY THE PLUGIN'S SERVER */
        /*****************************************/
        // When returning json, set 'return_value' field to following values :
        // - 0 : success
        // - 1 : not found
        // - 2 : parameter error
        // - 3 : other...
        // The field 'message' has to be present and will be sent to the client.

        const std::unordered_map<size_t, area::area_t> GetAreas(void) const;
        const json CreateArea(const area::action_t &, const area::reaction_t &);
        const json DeleteArea(const size_t &);
        const json UpdateAction(const size_t &, const area::action_t &);
        const json UpdateReaction(const size_t &, const area::reaction_t &);

    private:
        std::mutex _actions_mutex;
        std::unordered_map<size_t, Action *> _actions;
    };
}

#endif /* !ACTIONREACTIONMAIN_HPP_ */
