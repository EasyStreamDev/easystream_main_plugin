/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** SceneTransform
*/

#include "SceneTransform.hpp"

void es::utils::sceneTransform::SetSceneItemPosition(obs_sceneitem_t *sceneItem, float x, float y)
{
    struct vec2 pos = {x, y};
    obs_sceneitem_set_pos(sceneItem, &pos);
}

void es::utils::sceneTransform::SetSceneItemRotation(obs_sceneitem_t *sceneItem, float rotation)
{
    obs_sceneitem_set_rot(sceneItem, rotation);
}

void es::utils::sceneTransform::SetSceneItemScale(obs_sceneitem_t *sceneItem, float x, float y, bool FromCenter)
{
    struct vec2 ScreenSize = {1920, 1080};
    struct vec2 ScreenCenter = {0.5f * ScreenSize.x, 0.5f * ScreenSize.y};
    
    struct vec2 OriginalScale;
    obs_sceneitem_get_scale(sceneItem, &OriginalScale);

    struct vec2 scale = {OriginalScale.x * x, OriginalScale.y * y};

    if (FromCenter)
    {
        struct vec2 OriginalPos;
        obs_sceneitem_get_pos(sceneItem, &OriginalPos);

        struct vec2 translation = {
            (OriginalScale.x - OriginalScale.x * x) * ScreenCenter.x,
            (OriginalScale.y - OriginalScale.y * y) * ScreenCenter.y
            };

        OriginalPos.x += translation.x;
        OriginalPos.y += translation.y;
        obs_sceneitem_set_pos(sceneItem, &OriginalPos);
    }

    obs_sceneitem_set_scale(sceneItem, &scale);
}