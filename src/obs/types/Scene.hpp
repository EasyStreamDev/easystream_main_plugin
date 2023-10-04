#pragma once

#include "SceneItem.hpp"
#include "TextField.hpp"

namespace es::obs::types
{
    using UUID = std::string;

    class Scene : public SceneItem
    {
    public:
        Scene(obs_source_t *source, const std::optional<UUID> &parent_uuid = std::nullopt);
        ~Scene() = default;

        void update(void) final;
        const json getArchitecture(void) final;

    public:
        static bool retrieve_and_add_item(obs_scene_t *, obs_sceneitem_t *, void *);

    private:
        std::vector<SceneItem *> m_Children;
    };
}