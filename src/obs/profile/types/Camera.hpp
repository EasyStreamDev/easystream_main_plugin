#ifndef TYPES_CAMERA_HPP_
#define TYPES_CAMERA_HPP_

#include "SceneItem.hpp"

namespace es::obs::types
{
    using UUID = std::string;

    class Camera : public SceneItem
    {
    public:
        Camera(obs_source_t *source, const UUID &parent_uuid);
        ~Camera() = default;

        void update(void) final;
        const json getArchitecture(void) final;

    private:
        // Camera specific data
    };
}

#endif /* TYPES_CAMERA_HPP_ */