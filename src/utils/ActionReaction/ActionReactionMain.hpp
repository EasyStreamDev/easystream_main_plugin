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
#include "Actions/ActionKeyPressed.hpp"
#include "Actions/ActionAppLaunch.hpp"
#include "Actions/ActionWordDetect.hpp"

// Reaction includes
#include "Reactions/ReactionSceneSwitch.hpp"
#include "Reactions/ReactionToggleAudioCompressor.hpp"

namespace es
{
    class ActionReactionMain
    {
    public:
        const std::unordered_map<area::ActionType, std::function<Action *(Reaction *, const size_t &, const json &)>>
            ACTION_TYPE_TO_CREATE_FUNC = {
                {area::ActionType::KEY_PRESSED, [](Reaction *reaction, const size_t &area_id, const json &param) -> Action *
                 { return new ActionKeyPressed(reaction, area_id, param); }},
                {area::ActionType::APP_LAUNCH, [](Reaction *reaction, const size_t &area_id, const json &param) -> Action *
                 { return new ActionAppLaunch(reaction, area_id, param); }},
                {area::ActionType::WORD_DETECT, [](Reaction *reaction, const size_t &area_id, const json &param) -> Action *
                 { return new ActionWordDetect(reaction, area_id, param); }},
        };
        const std::unordered_map<area::ReactionType, std::function<Reaction *(const size_t &, const json &)>>
            REACTION_TYPE_TO_CREATE_FUNC = {
                {area::ReactionType::SCENE_SWITCH, [](const size_t &area_id, const json &param) -> Reaction *
                 { return new ReactionSceneSwitch(area_id, param); }},
                {area::ReactionType::TOGGLE_AUDIO_COMPRESSOR, [](const size_t &area_id, const json &param) -> Reaction *
                 { return new ReactionToggleAudioCompressor(area_id, param); }},
        };

    public:
        ActionReactionMain();
        ~ActionReactionMain();

        void run();

        void Update();
        void AddAction(Action *action);
        const bool RemoveAction(const size_t &id);
        const bool AreaExists(const size_t &id);

        /*****************************************/
        /* METHODS CALLED BY THE PLUGIN'S SERVER */
        /*****************************************/
        // When returning json, set 'return_value' field to following values :
        // - 0 : success
        // - 1 : not found
        // - 2 : parameter error
        // - 3 : other...
        // The field 'message' has to be present and will be sent to the client.

        const std::vector<area::area_t> GetAreas(void);
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