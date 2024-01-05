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
        obs_source_t *scene_source = nullptr;
        std::string uuid;

        if (param.contains("uuid"))
        {
            uuid = param["uuid"].get<std::string>();
            scene_source = obs_get_source_by_uuid(uuid.c_str());
        }
        else if (param.contains("name"))
        {
            std::string name = param["name"].get<std::string>();
            scene_source = obs_get_source_by_name(name.c_str());
            if (!scene_source)
            {
                throw AreaException("Scene with provided name does not exist.");
            }
            uuid = obs_source_get_uuid(scene_source);
        }

        if (!scene_source)
        {
            throw AreaException("UUID doesn't exist.");
        }
        if (!obs_source_is_scene(scene_source))
        {
            throw AreaException("UUID doesn't correspond to a scene object.");
        }

        _sceneToSwitch.uuid = uuid;
        _sceneToSwitch.name = obs_source_get_name(scene_source);

        obs_source_release(scene_source);
    }

    ReactionSceneSwitch::~ReactionSceneSwitch()
    {
    }

    void ReactionSceneSwitch::Resolve()
    {
        obs_source_t *scene_source = obs_get_source_by_uuid(_sceneToSwitch.uuid.c_str());

        if (!scene_source)
        {
            return;
        }
        if (!obs_source_is_scene(scene_source))
        {
            obs_source_release(scene_source);
            return;
        }

        // Changes the scene
        obs_frontend_set_current_scene(scene_source);
        obs_source_release(scene_source);
    }

    reaction_t ReactionSceneSwitch::ToStruct()
    {
        return {
            _id,
            "",
            ReactionType::SCENE_SWITCH,
            {
                {"name", _sceneToSwitch.name},
                {"uuid", _sceneToSwitch.uuid},
            },
        };
    }
}
