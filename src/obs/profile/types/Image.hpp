#ifndef TYPES_IMAGE_HPP_
#define TYPES_IMAGE_HPP_

#include "SceneItem.hpp"

namespace es::obs::types
{
    using UUID = std::string;

    class Image : public SceneItem
    {
    public:
        Image(obs_source_t *source, const UUID &parent_uuid);
        ~Image() = default;

        void update(void) final;
        const json getArchitecture(void) final;

    private:
        // Image source specific data
    };
}

#endif /* TYPES_IMAGE_HPP_ */