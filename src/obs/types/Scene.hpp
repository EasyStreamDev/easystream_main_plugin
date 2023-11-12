#pragma once

#include "SceneItem.hpp"
#include "TextField.hpp"
#include "Camera.hpp"
#include "Screen.hpp"

namespace es::obs::types
{
    using UUID = std::string;
    using SceneItemCtor = std::function<SceneItem *(obs_source_t *, const UUID &)>;

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