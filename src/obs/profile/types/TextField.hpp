#ifndef TYPES_TEXTFIELD_HPP_
#define TYPES_TEXTFIELD_HPP_

#include "SceneItem.hpp"

namespace es::obs::types
{
    using UUID = std::string;

    class TextField : public SceneItem
    {
    public:
        TextField(obs_source_t *source, const UUID &parent_uuid);
        ~TextField() = default;

        void update(void) final;
        const json getArchitecture(void) final;

    private:
        // Text field specific data
    };
}

#endif /* TYPES_TEXTFIELD_HPP_ */