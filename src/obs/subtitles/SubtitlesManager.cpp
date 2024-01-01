#include "SubtitlesManager.hpp"
#include "../../PluginManager.hpp"

namespace es::subtitles
{
    void SubtitlesManager::run(void *data)
    {
        try
        {
            blog(LOG_INFO, "### SubtitlesManager - STARTING...");
            m_PluginManager = static_cast<PluginManager *>(data);
            // transcription::TranscriptorManager *transcriptor_manager = m_PluginManager->GetTranscriptorManager();
            this->thread_sleep_ms(2000);
            blog(LOG_INFO, "### SubtitlesManager - STARTED");

            while (1)
            {
                {
                    std::unique_lock lock(_mtx);
                    _cVar.wait(lock, [this]()
                               { return !_TextFieldsTargets.empty() && !_transcripts.empty(); });
                }

                {
                    std::unique_lock lock(_mtxP);
                    while (!_transcripts.empty())
                    {
                        // subtitlesTranscript += _transcripts.front()._transcript;
                        auto tr = _transcripts.front();
                        setTextSubtitle(tr._micName, tr._transcript);
                        _transcripts.pop();
                    }
                }
            }
        }
        catch (...)
        {
            std::cerr << "Subtitles error" << std::endl;
        }
    }

    void SubtitlesManager::setTextSubtitle(std::string micName, const std::string &tr)
    {
        for (const auto &tf : _TextFieldsTargets)
        {
            if (tf.second.linkedMics.find(micName) == tf.second.linkedMics.npos)
                continue;
            obs_source_t *tf_source = obs_get_source_by_uuid(tf.first.c_str());
            obs_data_t *text_settings = obs_data_create();

            if (!tf_source || !text_settings)
                continue;

            obs_data_set_string(text_settings, "text", tr.c_str());
            obs_source_update(tf_source, text_settings);

            obs_data_release(text_settings);
            obs_source_release(tf_source);
        }
    }

    const json SubtitlesManager::getSubtitlesSettings(void) const
    {
        // json ret;
        std::vector<json> text_fields;

        {
            std::unique_lock lock(_mtxP);

            for (const auto &tf : _TextFieldsTargets)
            {
                if (tf.second.linkedMicsVec.empty())
                {
                    continue;
                }

                text_fields.push_back(json{
                    {"uuid", tf.second.uuid},
                    {"name", tf.second.name},
                    {"linked_mics", tf.second.linkedMicsVec}
                    // {"language", "to_be_ignored_for_now"},
                });
            }
        }
        // ret["length"] = text_fields.size();
        // ret["text_fields"] = text_fields;

        return json{
            {"length", text_fields.size()},
            {"text_fields", text_fields},
        };
    }

    void SubtitlesManager::pushSubtitles(std::string micName, std::string tr)
    {
        std::unique_lock lock(_mtxP);
        _transcripts.push(transcriptInfo{micName, tr});
        _cVar.notify_all();
    }

    void SubtitlesManager::setSubtitles(const std::string &uuid, const std::vector<std::string> &lMics, const std::string  &language)
    {
        std::string mics;
        obs_source_t *source = obs_get_source_by_uuid(uuid.c_str());
        text_field_data &tField = _TextFieldsTargets.emplace(uuid, text_field_data{uuid, obs_source_get_name(source), ""}).first->second;
        transcript::Transcriptor *tm = m_PluginManager->GetTranscriptor();

        for (const auto &m : lMics)
        {
            mics += m + ";";
            if (tField.linkedMics.find(m) == tField.linkedMics.npos)
            {
                tm->enableSubtitlesOnMic(m.c_str(), language);
            }
        }
        for (size_t i = 0, j = tField.linkedMics.find(';'); i < tField.linkedMics.size() && j != tField.linkedMics.npos; i = j + 1, j = tField.linkedMics.find(';', j + 1))
        {
            std::string m = tField.linkedMics.substr(i, j - i);

            if (mics.find(m) == mics.npos)
            {
                tm->disableSubtitlesOnMic(m);
            }
        }
        tField.linkedMics = mics;
        tField.linkedMicsVec = lMics;
        obs_source_release(source);
    }
}