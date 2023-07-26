#ifndef SUBTITLESMANAGER_HPP_
#define SUBTITLESMANAGER_HPP_

#include "../../Common.hpp"
#include "../../utils/Utils.hpp"
#include "../../Runnable.hpp"

namespace es
{
    class PluginManager;
}

namespace es::subtitles
{
    struct text_field_data
    {
        std::string uuid;
        std::string name;
    };

    class SubtitlesManager : public es::Runnable
    {
    public:
        void run(void *) final;
        void updateSubtitlesSettings(const std::string &, const bool &, const std::string &);
        const json getSubtitlesSettings(void) const;

    private:
        std::vector<text_field_data> m_TextFieldsTargets;
        PluginManager *m_PluginManager = nullptr;
    };
}

#endif /* !SUBTITLESMANAGER_HPP_ */