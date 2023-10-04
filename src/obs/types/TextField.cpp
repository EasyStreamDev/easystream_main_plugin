#include "TextField.hpp"

namespace es::obs::types
{
    TextField::TextField(obs_source_t *source, const UUID &parent_uuid)
        : SceneItem(source, parent_uuid)
    {
        this->update();
    }

    void TextField::update(void)
    {
        obs_source_t *source = obs_get_source_by_uuid(m_UUID.c_str());
        obs_scene_t *parent_scene = SceneItem::get_scene_from_uuid(m_ParentUUID.value());
        obs_sceneitem_t *scene_item = obs_scene_sceneitem_from_source(parent_scene, source);
        obs_transform_info transform_info;

        // Get scene item transform data
        obs_sceneitem_get_info(scene_item, &transform_info);
        // Store transform data
        this->m_transformData = SceneItem::transform_to_json(&transform_info);
        // Update name
        this->m_Name = obs_source_get_name(source);

        obs_scene_release(parent_scene);
        obs_source_release(source);
    }

    const json TextField::getArchitecture(void)
    {
        this->update();

        return json{
            {"kind", m_Kind},
            {"unv_kind", m_UnvKind},
            {"name", m_Name},
            {"uuid", m_UUID},
            {"transform", m_transformData},
        };
    }
}