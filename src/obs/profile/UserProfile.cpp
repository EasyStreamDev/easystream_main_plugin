#include "UserProfile.hpp"

namespace es::user
{
    UserProfile::UserProfile(IPluginManager *pm)
    {
        if (pm == nullptr)
        {
            perror("[UserProfile] Invalid pointer to plugin manager.");
            exit(1);
        }
        this->m_PluginManager = pm;
        this->update();
    }

    void UserProfile::update(void)
    {
        { // Video data
            video_t *video = obs_get_video();
            const video_output_info *video_info = video_output_get_info(video);

            // Frame rate
            m_VideoSettings.fps_numerator = video_info->fps_num;
            m_VideoSettings.fps_denominator = video_info->fps_den;

            // Dimensions
            m_VideoSettings.output_dimensions = {video_info->width, video_info->height};

            // Colors
            m_VideoSettings.colorspace_index = video_info->colorspace;
            m_VideoSettings.colorspace = get_video_colorspace_name(video_info->colorspace);
            m_VideoSettings.color_format_index = video_info->format;
            m_VideoSettings.color_format = get_video_format_name(video_info->format);
            m_VideoSettings.color_range_index = video_info->range;
            m_VideoSettings.color_range = get_video_range_name(video_info->format, video_info->range);
        }

        { // Audio data
            audio_t *audio = obs_get_audio();
            const audio_output_info *audio_info = audio_output_get_info(audio);

            m_AudioSettings.format_enum = audio_info->format;
            m_AudioSettings.sample_rate = audio_info->samples_per_sec;
            m_AudioSettings.channels = audio_output_get_channels(audio);
        }

        m_Inputs = es::utils::obs::listHelper::GetInputList();
        m_Outputs = es::utils::obs::listHelper::GetOutputList();

        m_EasystreamSettings = json{
            {"areas", this->getAreasSettings()},
            {"subtitles", this->getSubtitlesSettings()},
            {"compressors", this->getCompressorSettings()},
        };
    }

    const json UserProfile::getObsSettings(const bool &update)
    {
        std::vector<json> scenes_architectures = {};
        const auto scenes = m_PluginManager->GetSourceTracker()->getSceneMap();

        for (const auto &it : scenes)
        {
            scenes_architectures.push_back(it.second->getArchitecture());
        }

        if (update)
        {
            this->update();
        }

        return json{
            {"video_settings",
             {
                 {"frame_rate",
                  {
                      {"numerator", m_VideoSettings.fps_numerator},
                      {"denominator", m_VideoSettings.fps_denominator},
                      {"frame_rate", (float)m_VideoSettings.fps_numerator / (float)m_VideoSettings.fps_denominator},
                  }},
                 {"output_dimensions",
                  {
                      {"width", m_VideoSettings.output_dimensions.x},
                      {"height", m_VideoSettings.output_dimensions.y},
                  }},
                 {"colors",
                  {
                      {"colorspace", m_VideoSettings.colorspace},
                      {"colorspace_index", m_VideoSettings.colorspace_index},
                      {"color_range", m_VideoSettings.color_range},
                      {"color_range_index", m_VideoSettings.color_range_index},
                      {"color_format", m_VideoSettings.color_format},
                      {"color_format_index", m_VideoSettings.color_format_index},
                  }},
             }},
            {"audio_settings",
             {
                 {"format_index", m_AudioSettings.format_enum},
                 {"sample_rate", m_AudioSettings.sample_rate},
                 {"channels", m_AudioSettings.channels},
             }},
            {"inputs", m_Inputs},
            {"outputs", m_Outputs},
            {
                "architecture",
                {
                    {"length", scenes_architectures.size()},
                    {"scenes", scenes_architectures},
                },
            }};
    }

    const json UserProfile::getEeasystreamSettings(const bool &update)
    {
        if (update)
        {
            this->update();
        }

        return this->m_EasystreamSettings;
    }

    const json UserProfile::getAreasSettings(void)
    {
        std::vector<json> areas_vec;

        for (const auto &area : m_PluginManager->GetAreaMain()->GetAreas())
        {
            json area_data = {
                {"actReactId", area.id},
                {"isActive", area.is_active},
                {"action", {
                               //  {"actionId", area.action_data.id},
                               {"type", area::ActionTypeToString(area.action_data.type)},
                               {"params", area.action_data.params},
                           }},
                {"reaction", {
                                 //  {"reactionId", area.reaction_data.id},
                                 {"name", area.reaction_data.name},
                                 {"type", area::ReactionTypeToString(area.reaction_data.type)},
                                 {"params", area.reaction_data.params},
                             }},
            };
            areas_vec.push_back(std::move(area_data));
        }

        return json{
            {"length", areas_vec.size()},
            {"actReacts", areas_vec},
        };
    }

    const json UserProfile::getCompressorSettings(void)
    {
        std::vector<json> mics = es::utils::obs::listHelper::GetMicsList();
        const auto autoLevelerMap_ = m_PluginManager->GetSourceTracker()->getAudioMap();

        for (auto &m : mics)
        {
            std::shared_ptr<es::obs::AutoAudioLeveler> micAudioLeveler_ = autoLevelerMap_.find(m["micName"])->second;
            float tmpValue = micAudioLeveler_->getDesiredLevel() + 60;

            // Adding compressor level related to microphone
            m["level"] = 100.0 - floor((tmpValue * 100) / 60);
            m["isActive"] = micAudioLeveler_->isActive();
            m["uuid"] = micAudioLeveler_->GetUuid();
        }

        return json{
            {"length", mics.size()},
            {"mics", mics},
        };
    }

    const json UserProfile::getSubtitlesSettings(void)
    {
        return m_PluginManager->GetSubtitlesManager()->getSubtitlesSettings();
    }
}