#include "SubTitles.hpp"
#include "../../PluginManager.hpp"
#include "../speechRecognition/transcript/TranscriptorManager.hpp"

namespace es::obs::sub_titles
{
    void run(es::PluginManager *PluginManager)
    {
        blog(LOG_INFO, "### SubTitles - STARTING...");
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        blog(LOG_INFO, "### SubTitles - STARTED");

        while (1)
        {
            obs_source_t *text_source = obs_get_source_by_name("ES_SubTitles");
            if (!text_source)
            {
                continue;
            }

            if (auto _t = PluginManager->GetTranscriptorManager()->getTranscription())
            {
                std::string str;

                for (auto _w : _t->transcription)
                {
                    str += _w;
                }
                obs_data_t *text_settings = obs_data_create();
                obs_data_set_string(text_settings, "text", str.c_str());
                obs_source_update(text_source, text_settings);
                obs_data_release(text_settings);

                obs_source_release(text_source);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
    }
    catch (...)
    {
        std::cerr << "Subtitles error" << std::endl;
    }
}