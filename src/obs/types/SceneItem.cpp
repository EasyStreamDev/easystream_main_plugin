#include "SceneItem.hpp"

namespace es::obs::types
{
    SceneItem::SceneItem(obs_source_t *source, const std::optional<UUID> &parent_uuid)
        : m_UUID(obs_source_get_uuid(source)), m_ParentUUID(parent_uuid)
    {
        this->m_Kind = obs_source_get_id(source);
        this->m_UnvKind = obs_source_get_unversioned_id(source);
    }

    /**
     * Returns a pointer to the `obs_scene_t` object corresponding to the scene
     * with the same `UUID` as the one passed as parameter.
     * If not found returns `nullptr`.
     *
     * This function increments the reference to the scene object, therefore, the pointer
     * needs to be release using `obs_scene_release`.
     */
    obs_scene_t *SceneItem::get_scene_from_uuid(const std::string &uuid)
    {
        obs_source_t *source = obs_get_source_by_uuid(uuid.c_str());
        if (!source)
        {
            return nullptr;
        }
        obs_scene_t *scene = obs_get_scene_by_name(obs_source_get_name(source));

        obs_source_release(source);

        return scene;
    }

    /**
     * Converts an `obs_transform_info *` into a `nlohmann::json` object containing
     * the struct data if the pointer is not null.
     *
     * If the pointer is null, an empty `json` object is returned.
     */
    const json SceneItem::transform_to_json(obs_transform_info *t)
    {
        if (!t)
        {
            return json{};
        }

        return json{
            {
                "pos",
                {
                    {"x", t->pos.x},
                    {"y", t->pos.y},
                },
            },
            {"rot", t->rot},
            {
                "scale",
                {
                    {"x", t->scale.x},
                    {"y", t->scale.y},
                },
            },
            {"alignment", t->alignment},
            {
                "bounds",
                {
                    {"type", t->bounds_type},
                    {"alignment", t->bounds_alignment},
                    {
                        "values",
                        {
                            {"x", t->bounds.x},
                            {"y", t->bounds.y},
                        },
                    },
                },
            },
        };
    }
}