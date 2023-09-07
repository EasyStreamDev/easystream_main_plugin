/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** ReactionZoom
*/

#include "ReactionZoom.hpp"
#include "../../utils/SceneTransform.hpp"

namespace es::area
{
    ReactionZoom::ReactionZoom(const size_t &area_id, const std::string &name, const json &param)
        : Reaction(area_id, name, param)
    {
        ZoomRatio.x = param.at("zoom_ratio").at("x").get<float>();
        ZoomRatio.y = param.at("zoom_ratio").at("y").get<float>();
    }

    ReactionZoom::~ReactionZoom()
    {
    }

    void ReactionZoom::Resolve()
    {
        obs_source_t *sceneSource = obs_frontend_get_current_scene();
        if (!sceneSource)
            return;
        obs_scene_t *scene = obs_scene_from_source(sceneSource);
        if (!scene)
            return;

        obs_scene_enum_items(scene, [](obs_scene_t *scene, obs_sceneitem_t *sceneItem, void *param) {
            struct vec2 *Ratio = (struct vec2 *)param;
            utils::sceneTransform::SetSceneItemScale(sceneItem, Ratio->x, Ratio->y);
            return true;
        }, &ZoomRatio);
    }

    reaction_t ReactionZoom::ToStruct()
    {
        return {
            _id,
            "",
            es::area::ReactionType::ZOOM,
            {{"zoom_ratio", {{"x", ZoomRatio.x}, {"y", ZoomRatio.y}}}}};
    }
}

