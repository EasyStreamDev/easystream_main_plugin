#pragma once

#include "../../../Common.hpp"

namespace es::obs::types
{
    using UUID = std::string;

    class SceneItem
    {
    public:
        SceneItem(obs_source_t *source, const std::optional<UUID> &parent_uuid = std::nullopt);
        ~SceneItem() = default;

        virtual void update(void) = 0;
        virtual const json getArchitecture(void) = 0;

        inline const std::string &getName(void) const { return m_Name; };
        void setName(const std::string &name) { m_Name = name; }

    protected:
        static obs_scene_t *get_scene_from_uuid(const UUID &uuid);
        static const json transform_to_json(obs_transform_info *);

    protected:
        const UUID m_UUID;
        const std::optional<UUID> m_ParentUUID;

        std::string m_Name;
        std::string m_Kind;
        std::string m_UnvKind;
        json m_transformData;
    };
}
