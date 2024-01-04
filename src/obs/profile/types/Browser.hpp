#ifndef TYPES_BROWSER_HPP_
#define TYPES_BROWSER_HPP_

#include "SceneItem.hpp"

namespace es::obs::types
{
    using UUID = std::string;

    class Browser : public SceneItem
    {
    public:
        Browser(obs_source_t *source, const UUID &parent_uuid);
        ~Browser() = default;

        void update(void) final;
        const json getArchitecture(void) final;

    private:
        // Browser source specific data
    };
}

#endif /* TYPES_BROWSER_HPP_ */