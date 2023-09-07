/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** ReactionMove
*/

#include "ReactionMove.hpp"
#include "../../utils/SceneTransform.hpp"

namespace es::area
{
    ReactionMove::ReactionMove(const size_t &area_id, const std::string &name, const json &param)
        : Reaction(area_id, name, param)
    {
        TranslationIncrease = param.at("translation_increase").get<float>();
        Direction = param.at("direction").get<int>();
    }

    ReactionMove::~ReactionMove()
    {
    }

    struct MoveInfo
    {
        float TranslationIncrease;
        int Direction;
    };


    void ReactionMove::Resolve()
    {
        obs_source_t *sceneSource = obs_frontend_get_current_scene();
        if (!sceneSource)
            return;
        obs_scene_t *scene = obs_scene_from_source(sceneSource);
        if (!scene)
            return;

        MoveInfo info;
        info.TranslationIncrease = TranslationIncrease;
        info.Direction = Direction;


        obs_scene_enum_items(scene, [](obs_scene_t *scene, obs_sceneitem_t *sceneItem, void *param) {
            float Translation = ((MoveInfo *)param)->TranslationIncrease;
            int Direction = ((MoveInfo *)param)->Direction;

            struct vec2 OriginalPosition;
            obs_sceneitem_get_pos(sceneItem, &OriginalPosition);

            int x = int(Direction == 2) - int(Direction == 3);
            int y = int(Direction == 0) - int(Direction == 1);

            utils::sceneTransform::SetSceneItemPosition(sceneItem, OriginalPosition.x + (Translation * x), OriginalPosition.y + (Translation * y));
            return true;
        }, &info);
    }

    reaction_t ReactionMove::ToStruct()
    {
        return {
            _id,
            "",
            es::area::ReactionType::MOVE,
            {{"translation_increase", TranslationIncrease}, {"direction", Direction}}};
    }
}

