/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** AreaManager
*/

#ifndef AREAMANAGER_HPP_
#define AREAMANAGER_HPP_

#include "../Runnable.hpp"
#include "../Common.hpp"

// Actions includes
#include "actions/ActionKeyPressed.hpp"
#include "actions/ActionAppLaunch.hpp"
#include "actions/ActionWordDetect.hpp"

// Reaction includes
#include "reactions/ReactionSceneSwitch.hpp"
#include "reactions/ReactionToggleAudioCompressor.hpp"
#include "reactions/ReactionRecordingStart.hpp"
#include "reactions/ReactionRecordingStop.hpp"
#include "reactions/ReactionStreamStart.hpp"
#include "reactions/ReactionStreamStop.hpp"
// #include <shared_mutex>
#include <mutex>
#include <queue>

namespace es::area
{
    class AreaManager : es::Runnable
    {
    public:
        const std::unordered_map<area::ActionType, std::function<Action *(Reaction *, const size_t &, const json &)>>
            ACTION_TYPE_TO_CREATE_FUNC = {
                {area::ActionType::KEY_PRESSED, [](Reaction *reaction, const size_t &area_id, const json &param) -> Action *
                 { return new ActionKeyPressed(reaction, area_id, param); }},
                {area::ActionType::APP_LAUNCH, [](Reaction *reaction, const size_t &area_id, const json &param) -> Action *
                 { return new ActionAppLaunch(reaction, area_id, param); }},
                {area::ActionType::WORD_DETECT, [this](Reaction *reaction, const size_t &area_id, const json &param) -> Action *
                 { return new ActionWordDetect(reaction, area_id, param); }},
            };
        const std::unordered_map<area::ReactionType, std::function<Reaction *(const size_t &, const std::string &, const json &)>>
            REACTION_TYPE_TO_CREATE_FUNC = {
                {area::ReactionType::SCENE_SWITCH, [](const size_t &area_id, const std::string &name, const json &param) -> Reaction *
                 { return new ReactionSceneSwitch(area_id, name, param); }},
                {area::ReactionType::TOGGLE_AUDIO_COMPRESSOR, [](const size_t &area_id, const std::string &name, const json &param) -> Reaction *
                 { return new ReactionToggleAudioCompressor(area_id, name, param); }},
                {area::ReactionType::START_RECORDING, [](const size_t &area_id, const std::string &name, const json &param) -> Reaction *
                 { return new ReactionRecordingStart(area_id, name, param); }},
                {area::ReactionType::STOP_RECORDING, [](const size_t &area_id, const std::string &name, const json &param) -> Reaction *
                 { return new ReactionRecordingStop(area_id, name, param); }},
                {area::ReactionType::START_STREAMING, [](const size_t &area_id, const std::string &name, const json &param) -> Reaction *
                 { return new ReactionStreamStart(area_id, name, param); }},
                {area::ReactionType::STOP_STREAMING, [](const size_t &area_id, const std::string &name, const json &param) -> Reaction *
                 { return new ReactionStreamStop(area_id, name, param); }},
            };

    public:
        AreaManager();
        ~AreaManager();

        void run(void *) override;

        void Update();
        void AddAction(Action *action);
        void AddWords(const std::string &);
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
        std::mutex _mtx;
        std::queue<std::string> _words;
        es::IPluginManager *_pm;
    };
}

#endif /* !AREAMANAGER_HPP_ */
