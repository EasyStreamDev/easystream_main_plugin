#ifndef USERPROFILE_HPP_
#define USERPROFILE_HPP_

#include "../../Common.hpp"
#include "../../utils/Obs.hpp"
#include "../../IPluginManager.hpp"
#include "../../area/AreaManager.hpp"
#include "../../obs/subtitles/SubtitlesManager.hpp"

namespace es::user
{
    struct vector2d
    {
        uint32_t x;
        uint32_t y;
    };

    // Check https://docs.obsproject.com/reference-core#c.obs_reset_video
    typedef struct video_settings_s
    {
        /* Frame rate */

        uint32_t fps_numerator;
        uint32_t fps_denominator;

        /* Output dimension */

        vector2d output_dimensions;

        /* Colors */

        std::string colorspace;
        uint32_t colorspace_index;
        std::string color_range;
        uint32_t color_range_index;
        std::string color_format;
        uint32_t color_format_index;
    } video_settings_t;

    typedef struct audio_settings_s
    {
        audio_format format_enum;
        uint32_t sample_rate;
        int channels;
    } audio_settings_t;

    class UserProfile
    {
    public:
        UserProfile(IPluginManager *);
        void update(void);
        const json getObsSettings(const bool &update = true);
        const json getEeasystreamSettings(const bool &update = true);
        const json getAreasSettings(void);
        const json getCompressorSettings(void);
        const json getSubtitlesSettings(void);

    private:
        IPluginManager *m_PluginManager = nullptr;
        video_settings_t m_VideoSettings;
        audio_settings_t m_AudioSettings;
        std::vector<json> m_Inputs;
        std::vector<json> m_Outputs;
        json m_EasystreamSettings;
    };
}

#endif /* !USERPROFILE_HPP_ */