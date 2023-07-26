#include "SubtitlesManager.hpp"
#include "../../PluginManager.hpp"
#include "../speechRecognition/transcript/TranscriptorManager.hpp"

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

                // if (!transcriptor_manager || m_TextFieldsTargets.empty())
                // {
                //     transcriptor_manager = m_PluginManager->GetTranscriptorManager();
                //     this->thread_sleep_ms(2000);
                //     continue;
                // }

                // // Checking if any transcription was made.
                // if (auto _t = transcriptor_manager->getTranscription())
                // {
                //     std::string text_data;

                //     for (auto _w : _t->transcription)
                //     {
                //         text_data += _w;
                //     }

                //     for (const auto &tf : m_TextFieldsTargets)
                //     {
                //         obs_source_t *tf_source = obs_get_source_by_uuid(tf.uuid.c_str());
                //         obs_data_t *text_settings = obs_data_create();

                //         if (!tf_source || !text_settings)
                //         {
                //             continue;
                //         }

                //         // Set text to textfield
                //         obs_data_set_string(text_settings, "text", text_data.c_str());
                //         obs_source_update(tf_source, text_settings);

                //         // Release sources
                //         obs_data_release(text_settings);
                //         obs_source_release(tf_source);
                //     }
                // }

                this->thread_sleep_ms(2000);
            }
        }
        catch (...)
        {
            std::cerr << "Subtitles error" << std::endl;
        }
    }

    void SubtitlesManager::updateSubtitlesSettings(const std::string &uuid, const bool &enable, const std::string &name)
    {
        if (enable)
        {
            this->m_TextFieldsTargets.push_back({uuid, name});
        }
        else
        {
            // To modify
            m_TextFieldsTargets.erase(
                std::remove_if(
                    m_TextFieldsTargets.begin(),
                    m_TextFieldsTargets.end(),
                    [uuid](const text_field_data &tf)
                    {
                        return tf.uuid == uuid;
                    }),
                m_TextFieldsTargets.end());
        }
    }

    const std::vector<json> SubtitlesManager::getSubtitlesSettings(void) const
    {
        std::vector<json> ret;

        for (const text_field_data tf : m_TextFieldsTargets)
        {
            ret.push_back({
                {"uuid", tf.uuid},
                {"name", tf.name},
                // {"language", "to_be_ignored_for_now"},
            });
        }

        return ret;
    }
}