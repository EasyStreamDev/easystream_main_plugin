/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** SceneTransform
*/

#ifndef SCENETRANSFORM_HPP_
#define SCENETRANSFORM_HPP_

#include "../Common.hpp"

namespace es
{
    namespace utils
    {
        namespace sceneTransform
        {
            void SetSceneItemPosition(obs_sceneitem_t *sceneItem, float x, float y);
            void SetSceneItemRotation(obs_sceneitem_t *sceneItem, float rotation);
            void SetSceneItemScale(obs_sceneitem_t *sceneItem, float x, float y, bool FromCenter = true);
        }
    }
}

#endif /* !SCENETRANSFORM_HPP_ */
