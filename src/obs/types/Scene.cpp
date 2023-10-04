#include "Scene.hpp"

namespace es::obs::types
{
    Scene::Scene(obs_source_t *source, const std::optional<UUID> &parent_uuid)
        : SceneItem(source, parent_uuid)
    {
        this->update();
    }

    void Scene::update(void)
    {
        obs_source_t *source = obs_get_source_by_uuid(m_UUID.c_str());
        obs_scene_t *scene = nullptr;

        if (!source)
        {
            return;
        }

        m_Name = obs_source_get_name(source);
        scene = obs_get_scene_by_name(m_Name.c_str());

        if (!scene)
        {
            obs_source_release(source);
            return;
        }

        // Check if this scene is within another
        if (m_ParentUUID.has_value())
        {
            std::cerr << "Child scene detected: updating - " << m_Name << std::endl;
            obs_scene_t *parent_scene = Scene::get_scene_from_uuid(m_ParentUUID.value());
            obs_sceneitem_t *scene_item = obs_scene_sceneitem_from_source(parent_scene, source);
            obs_scene_release(parent_scene);

            if (scene_item)
            {
                obs_transform_info transform_info;

                obs_sceneitem_get_info(scene_item, &transform_info);
                m_transformData = SceneItem::transform_to_json(&transform_info);

                obs_sceneitem_release(scene_item);
            }
        }
        // Otherwise it is a parent/root scene
        else
        {
            std::cerr << "Root scene detected: updating - " << m_Name << std::endl;

            SceneItem *scene_item = nullptr;

            for (auto it = m_Children.begin(); it != m_Children.end(); ++it)
            {
                scene_item = *it;
                delete scene_item;
            }
            std::cerr << "--- Updating composition" << m_Name << std::endl;
            m_Children.clear();
            obs_scene_enum_items(scene, Scene::retrieve_and_add_item, this);
        }

        obs_scene_release(scene);
        obs_source_release(source);
    }

    const json Scene::getArchitecture(void)
    {
        this->update();

        if (m_ParentUUID.has_value())
        {
            return json{
                {"kind", m_Kind},
                {"unv_kind", m_UnvKind},
                {"name", m_Name},
                {"uuid", m_UUID},
                {"transform", m_transformData},
            };
        }

        std::vector<json> children_archs;

        for (SceneItem *si : m_Children)
        {
            children_archs.push_back(si->getArchitecture());
        }

        return json{
            {"kind", m_Kind},
            {"unv_kind", m_UnvKind},
            {"name", m_Name},
            {"uuid", m_UUID},
            {"children", children_archs},
        };
    }

    bool Scene::retrieve_and_add_item(obs_scene_t *scene, obs_sceneitem_t *scene_item, void *private_data)
    {
        Scene *parent = static_cast<Scene *>(private_data);
        obs_source_t *source = obs_sceneitem_get_source(scene_item);
        const std::string unv_kind = obs_source_get_unversioned_id(source);

        std::cerr << "--- --- Child detected with kind: " << unv_kind << std::endl;
        if (unv_kind == "scene")
        {
            parent->m_Children.push_back(new Scene(source, parent->m_UUID));
        }
        else if (unv_kind == "text_ft2_source")
        {
            parent->m_Children.push_back(new TextField(source, parent->m_UUID));
        }

        // parent->m_Children.push_back(SceneItem(source));
        return true;
    }
}