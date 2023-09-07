/*
** easyStream EIP, 2021
** easystream_main_plugin
** File description:
** topAudioMic
*/

#ifndef TOPAUDIOMIC_HPP_
#define TOPAUDIOMIC_HPP_

// Global
#include "../../Common.hpp"

// Linked
#include <iostream>

namespace es::obs
{
    struct VolumesData
    {
        double StartTime;
        float Sum = 0;
        float Number = 0;
    };

    class TopAudioMic
    {
    public:
        TopAudioMic();
        ~TopAudioMic();

        static void InputAudioCaptureCallback(void *priv_data, obs_source_t *, const struct audio_data *data, bool muted);
        float CalculateAudioLevel(const struct audio_data *data, bool muted);

        void AddNewAudioSource(obs_source_t *source);
        void AddNewVideoSource(obs_source_t *source);

        void UpdateTopMic();

    private:
        std::map<std::string, VolumesData> _sourcesVolumes;

        std::vector<std::string> _sourcesVideo;
    };
}

#endif /* !TOPAUDIOMIC_HPP_ */
