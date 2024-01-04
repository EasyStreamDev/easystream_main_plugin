#ifndef TYPES_COLOR_HPP_
#define TYPES_COLOR_HPP_

#include "SceneItem.hpp"

namespace es::obs::types
{
    using UUID = std::string;

    class Color : public SceneItem
    {
    public:
        Color(obs_source_t *source, const UUID &parent_uuid);
        ~Color() = default;

        void update(void) final;
        const json getArchitecture(void) final;

    private:
        // Color source specific data
    };
}

#endif /* TYPES_COLOR_HPP_ */