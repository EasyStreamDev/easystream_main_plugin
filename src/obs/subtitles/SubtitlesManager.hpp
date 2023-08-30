#ifndef SUBTITLESMANAGER_HPP_
#define SUBTITLESMANAGER_HPP_

#include "../../Common.hpp"
#include "../../utils/Utils.hpp"
#include "../../Runnable.hpp"
#include <queue>
#include <mutex>

namespace es
{
    class PluginManager;
}

namespace es::subtitles
{
    struct text_field_data
    {
        std::string uuid;
        // std::string name;
        std::string linkedMics = "";
        // std::string actualTxt;
    };

    struct transcriptInfo
    {
        const char *_micName;
        std::string _transcript;
    };

    class SubtitlesManager : public es::Runnable
    {
    public:
        void run(void *) final;
        void updateSubtitlesSettings(const std::string &, const bool &, const std::string &);
        const std::vector<json> getSubtitlesSettings(void) const;
        void pushSubtitles(const char *micName, std::string transcript);
        void setSubtitles(const std::string &uuid, const std::vector<std::string> & lMics);
    private:
        void setTextSubtitle(const char *micName, const std::string &tr);


        std::vector<text_field_data> m_TextFieldsTargets;
        std::unordered_map<std::string, text_field_data> _TextFieldsTargets;
        PluginManager *m_PluginManager = nullptr;
        std::queue<transcriptInfo> _transcripts;
        std::mutex _mtx;
    };
}

#endif /* !SUBTITLESMANAGER_HPP_ */