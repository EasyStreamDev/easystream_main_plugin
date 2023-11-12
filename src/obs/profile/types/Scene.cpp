#include "Scene.hpp"

namespace es::obs::types
{
    static const std::unordered_map<std::string, SceneItemCtor> MAP_UNV_KIND_TO_TYPE_CTOR = {
        {
            "scene",
            [](obs_source_t *source, const UUID &parent_uuid)
            { return new Scene(source, parent_uuid); },
        },
        {
            "text_ft2_source",
            [](obs_source_t *source, const UUID &parent_uuid)
            { return new TextField(source, parent_uuid); },
        },
        {
            "v4l2_input",
            [](obs_source_t *source, const UUID &parent_uuid)
            { return new Camera(source, parent_uuid); },
        },
        {
            "xshm_input",
            [](obs_source_t *source, const UUID &parent_uuid)
            { return new Screen(source, parent_uuid); },
        },
        {
            "color_source",
            [](obs_source_t *source, const UUID &parent_uuid)
            { return new Color(source, parent_uuid); },
        },
        {
            "image_source",
            [](obs_source_t *source, const UUID &parent_uuid)
            { return new Image(source, parent_uuid); },
        },
        {
            "browser_source",
            [](obs_source_t *source, const UUID &parent_uuid)
            { return new Browser(source, parent_uuid); },
        },
    };

    Scene::Scene(obs_source_t *source, const std::optional<UUID> &parent_uuid)
        : SceneItem(source, parent_uuid)
    {
        this->update();
    }

    void Scene::update(void)
    {
        OBSSourceAutoRelease source = obs_get_source_by_uuid(m_UUID.c_str());
        OBSSceneAutoRelease scene = nullptr;

        if (!source)
        {
            return;
        }

        m_Name = obs_source_get_name(source);
        scene = obs_get_scene_by_name(m_Name.c_str());

        if (!scene)
        {
            return;
        }

        // Check if this scene is within another
        if (m_ParentUUID.has_value())
        {
            std::cerr << "Child scene detected: updating - " << m_Name << std::endl;
            OBSSceneAutoRelease parent_scene = Scene::get_scene_from_uuid(m_ParentUUID.value());
            OBSSceneItemAutoRelease scene_item = obs_scene_sceneitem_from_source(parent_scene.Get(), source);

            if (scene_item.Get())
            {
                obs_transform_info transform_info;

                obs_sceneitem_get_info(scene_item.Get(), &transform_info);
                m_transformData = SceneItem::transform_to_json(&transform_info);
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
        obs_source_t *source = obs_sceneitem_get_source(scene_item); // Does not need to be released
        const std::string unv_kind = obs_source_get_unversioned_id(source);
        const auto ctor_iterator = MAP_UNV_KIND_TO_TYPE_CTOR.find(unv_kind);

        if (ctor_iterator != MAP_UNV_KIND_TO_TYPE_CTOR.end())
        {
            parent->m_Children.push_back(ctor_iterator->second(source, parent->m_UUID));
        }
        else
        {
            std::cerr << "[Scene::retrieve_and_add_item] Child detected with unsupported kind: " << unv_kind << std::endl;
        }

        return true;
    }
}