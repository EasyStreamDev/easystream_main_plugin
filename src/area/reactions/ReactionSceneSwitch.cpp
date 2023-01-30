/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ReactionSceneSwitch
*/

#include "ReactionSceneSwitch.hpp"

namespace es::area
{
    ReactionSceneSwitch::ReactionSceneSwitch(const size_t &area_id, const std::string &name, const json &param)
        : Reaction(area_id, name, param)
    {
        _sceneToSwitch = param["scene"].get<std::string>();
    }

    ReactionSceneSwitch::~ReactionSceneSwitch()
    {
    }

    void ReactionSceneSwitch::Resolve()
    {
        obs_frontend_set_current_scene(obs_scene_get_source(obs_get_scene_by_name(_sceneToSwitch.c_str())));
    }

    reaction_t ReactionSceneSwitch::ToStruct()
    {
        return {
            _id,
            "",
            ReactionType::SCENE_SWITCH,
            {{"scene", _sceneToSwitch}}};
    }
}
