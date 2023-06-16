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
    class SubtitlesManager : public es::Runnable
    {
    public:
        void run(void *) final;
        void updateSubtitlesSettings(const std::string &, const bool &);
        const std::vector<json> getSubtitlesSettings(void) const;

    private:
        std::vector<std::string> m_TextFieldsTargets;
        PluginManager *m_PluginManager = nullptr;
    };
}

#endif /* !SUBTITLESMANAGER_HPP_ */