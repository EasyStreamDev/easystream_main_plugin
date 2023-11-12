#ifndef TYPES_SCREEN_HPP_
#define TYPES_SCREEN_HPP_

#include "SceneItem.hpp"

namespace es::obs::types
{
    using UUID = std::string;

    class Screen : public SceneItem
    {
    public:
        Screen(obs_source_t *source, const UUID &parent_uuid);
        ~Screen() = default;

        void update(void) final;
        const json getArchitecture(void) final;

    private:
        // Screen specific data
    };
}

#endif /* TYPES_SCREEN_HPP_ */