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
        std::string name;
        std::string linkedMics = "";
        std::vector<std::string> linkedMicsVec;
        // std::string actualTxt;
    };

    struct transcriptInfo
    {
        std::string _micName;
        std::string _transcript;
    };

    class SubtitlesManager : public es::Runnable
    {
    public:
        void run(void *) final;
        void updateSubtitlesSettings(const std::string &, const bool &, const std::string &);
        json getSubtitlesSettings(void) const;
        void pushSubtitles(std::string micName, std::string transcript);
        void setSubtitles(const std::string &uuid, const std::vector<std::string> &lMics);

    private:
        void setTextSubtitle(std::string micName, const std::string &tr);

        std::vector<text_field_data> m_TextFieldsTargets;
        std::unordered_map<std::string, text_field_data> _TextFieldsTargets;
        PluginManager *m_PluginManager = nullptr;
        std::queue<transcriptInfo> _transcripts;
        std::mutex _mtx;
        mutable std::mutex _mtxP;
        std::condition_variable _cVar;
    };
}

#endif /* !SUBTITLESMANAGER_HPP_ */