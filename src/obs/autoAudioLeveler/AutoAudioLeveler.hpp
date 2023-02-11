/*
** easyStream EIP, 2021
** easystream_main_plugin
** File description:
** autoAudioLeveler
*/

#ifndef AUTOAUDIOLEVEL_HPP_
#define AUTOAUDIOLEVEL_HPP_

// Global
#include "../../Common.hpp"

// Linked
#include <iostream>

namespace es::obs
{
    class AutoAudioLeveler
    {
    private:
        obs_source_t *_source;
        float _levelToGo;
        std::clock_t _startTime;
        std::list<float> _levels;

        float _desiredAudioLevel;
        float _minDetectLevel;
        bool _active;
        bool _toStop;

    public:
        AutoAudioLeveler(obs_source_t *input);
        ~AutoAudioLeveler();

        static void InputAudioCaptureCallback(void *priv_data, obs_source_t *, const struct audio_data *data, bool muted);

        void stopCapture();
        float computeLerp(float audioVolume);
        void ComputeAudioLevel(float audioLevelMul, float audioVolume);
        float CalculateAudioLevel(const struct audio_data *data, bool muted);
        const float &getDesiredLevel() const;
        void setDesiredLevel(const float &);
        bool isActive() const;
        void SetActive(bool);
    };
}

#endif /* !AUTOAUDIOLEVEL_HPP_ */
