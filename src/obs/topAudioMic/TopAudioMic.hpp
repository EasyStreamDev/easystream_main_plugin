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
#include "../../server/include/CommunicationErrors.hpp"

// Linked
#include <iostream>

namespace es::obs
{
    struct MicDisplayLink
    {
        std::string MicUuid;
        std::vector<std::string> DisplaysUuid;
    };

    struct VolumesData
    {
        double Time;
        float Volume = 0;
    };

    class TopAudioMic
    {
    public:
        static void InputAudioCaptureCallback(void *priv_data, obs_source_t *, const struct audio_data *data, bool muted);

    public:
        TopAudioMic();
        ~TopAudioMic();

        float CalculateAudioLevel(const struct audio_data *data, bool muted);

        void AddMicDisplayLinks(MicDisplayLink Link);
        void RemoveMicDisplayLinks(std::string MicUuid);
        void UpdateMicDisplayLinks(std::string MicUuid, std::vector<std::string> NewDisplaysUuid);

        // void AddNewAudioSource(obs_source_t *source);
        // void AddNewVideoSource(obs_source_t *source);

        void UpdateTopMic();

        inline const std::unordered_map<std::string, std::vector<std::string>> GetLinks() const { return m_MicDisplayLinks; };

    private:
        std::unordered_map<std::string, std::vector<std::string>> m_MicDisplayLinks;
        std::vector<std::pair<std::string, std::vector<VolumesData>>> m_MicsVolumes;
    };
}

#endif /* !TOPAUDIOMIC_HPP_ */