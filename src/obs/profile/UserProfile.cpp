#include "UserProfile.hpp"

namespace es::user
{
    UserProfile::UserProfile()
    {
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

        { /* DISPLAY PART */

            std::cout << "\n------------------------------" << std::endl;
            std::cout << "--------VIDEO SETTINGS--------" << std::endl;
            std::cout << "------------------------------" << std::endl;
            std::cout << "Frame Rate:" << std::endl;
            std::cout << "\tNumerator: " << m_VideoSettings.fps_numerator << std::endl;
            std::cout << "\tDenominator: " << m_VideoSettings.fps_denominator << std::endl;
            std::cout << "\tRate: " << (float)m_VideoSettings.fps_numerator / (float)m_VideoSettings.fps_denominator << std::endl;
            std::cout << "Output dimensions: "
                      << m_VideoSettings.output_dimensions.x << "x"
                      << m_VideoSettings.output_dimensions.y << std::endl;
            std::cout << "Colors:" << std::endl;
            std::cout << "\tColorspace: " << m_VideoSettings.colorspace << std::endl;
            std::cout << "\tColor range: " << m_VideoSettings.color_range << std::endl;
            std::cout << "\tColor format: " << m_VideoSettings.color_format << std::endl;

            std::cout << "\n------------------------------" << std::endl;
            std::cout << "--------AUDIO SETTINGS--------" << std::endl;
            std::cout << "------------------------------" << std::endl;
            std::cout << "Format index: " << m_AudioSettings.format_enum << std::endl;
            std::cout << "Sample rate: " << m_AudioSettings.sample_rate << std::endl;
            std::cout << "Channels: " << m_AudioSettings.channels << std::endl;

            std::cout << "\n------------------------------" << std::endl;
            std::cout << "------------INPUTS------------" << std::endl;
            std::cout << "------------------------------" << std::endl;
            for (const json &in_data : m_Inputs)
            {
                std::cout << "{" << std::endl;
                std::cout << "\tname: " << in_data["inputName"] << std::endl;
                std::cout << "\tkind: " << in_data["inputKind"] << std::endl;
                std::cout << "\tunv_kind: " << in_data["unversionedInputKind"] << std::endl;
                std::cout << "}" << std::endl;
            }

            std::cout << "\n------------------------------" << std::endl;
            std::cout << "------------OUTPUTS------------" << std::endl;
            std::cout << "------------------------------" << std::endl;
            for (const json &out_data : m_Outputs)
            {
                std::cout << "{" << std::endl;
                std::cout << "\tname: " << out_data["outputName"] << std::endl;
                std::cout << "\tkind: " << out_data["outputKind"] << std::endl;
                std::cout << "}" << std::endl;
            }
        }

        { // Tests
          // config_t *profile_config = obs_frontend_get_profile_config();

            // for (int i = 0; i < config_num_sections(profile_config); ++i)
            // {
            //     std::cout << "\tSection: " << config_get_section(profile_config, i) << std::endl;
            // }
            // const char *v = config_get_string(profile_config, "Video", "colorspace");
            // if (v != NULL)
            // {
            //     std::cout << "############### " << v << std::endl;
            // }
        }
    }
}