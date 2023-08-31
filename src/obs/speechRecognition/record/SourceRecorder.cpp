/*
** EPITECH PROJECT, 2021
** easystream_main_plugin
** File description:
** SourceRecorder
*/

#include "SourceRecorder.hpp"
#include <obs.h>

namespace es::obs
{
    SourceRecorder::SourceRecorder()
        : _headerWav(false), _isActive(false)
    {
    }

    SourceRecorder::~SourceRecorder()
    {
        _outFile.close();
        std::filesystem::remove_all(_temporaryPath);
    }

    void SourceRecorder::init(obs_source_t *input, const std::string &micName)
    {
        _temporaryPath = std::filesystem::temp_directory_path();
        _source = input;
        _micName = micName;
        _temporaryPath += "/tempEasyStreamAudio";
        // std::cout << _micName << std::endl;
        std::filesystem::create_directory(_temporaryPath);
        clearMicName();
        // _outFile.open(_temporaryPath.string()+ "/output.wav", std::ios::binary);
        const struct audio_output_info *obs_audio = audio_output_get_info(obs_get_audio());

        if (!obs_audio || !input)
        {
            blog(LOG_INFO, "### [SourceRecorder] - Error getting source data.");
            return;
        }

        // exit(84);
        resample_info resample_to = {48000, AUDIO_FORMAT_16BIT, SPEAKERS_MONO};

        // blog(LOG_INFO, "### [MMMMMMMMMMMMMMMMMMMMMMMMMMM] %d, %d, %d", resample_to.samples_per_sec, resample_to.format, resample_to.speakers);
        if (obs_audio->samples_per_sec != resample_to.samples_per_sec || obs_audio->format != resample_to.format || obs_audio->speakers != resample_to.speakers)
        {
            resample_info src = {
                obs_audio->samples_per_sec,
                obs_audio->format,
                obs_audio->speakers};

            this->_resampler = audio_resampler_create(&resample_to, &src);
            if (!this->_resampler)
            {
                throw std::string("Failed to create audio resampler");
            }
            // blog(LOG_INFO, "### [MMMMMMMMMMMMMMMMMMMMMMMMMMM] %d, %d, %d", obs_audio->samples_per_sec, obs_audio->format, obs_audio->speakers);
        }

        obs_source_add_audio_capture_callback(_source, InputAudioCaptureCallback, this);
        blog(LOG_INFO, "### [SourceRecorder] - Recorder set for input: %s", obs_source_get_name(_source));
    }

    void SourceRecorder::run(void *)
    {
        // std::cout << _micName << " BEFORE        RECORDING            " << std::endl;
        // obs_source_add_audio_capture_callback(_source, InputAudioCaptureCallback, this);
        // std::cout << _micName << " STARTTTINNNNNNNNNNNNNNG        RECORDING            " << std::endl;
        while (1)
        {
            /* code */
        };
    }

    void SourceRecorder::InputAudioCaptureCallback(void *priv_data, obs_source_t *source, const struct audio_data *data, bool muted)
    {
        SourceRecorder *self = static_cast<SourceRecorder *>(priv_data);

        if (!self->_isActive)
        {
            return;
        }

        if (!data || !data->frames || !self)
        {
            return;
        }

        unsigned int size = 0;
        uint8_t *out[MAX_AV_PLANES];
        uint32_t out_frames;
        uint64_t ts_offset;
        bool success = audio_resampler_resample(
            self->_resampler,
            out,
            &out_frames,
            &ts_offset,
            (const uint8_t *const *)data->data,
            data->frames);

        if (!success || !out[0])
        {
            blog(LOG_INFO, "### [SourceRecorder] - Failed resampling input audio data.");
            return;
        }
        size = sizeof(int16_t) * out_frames * self->_wavFile.numOfChan;

        if (muted)
        {
            memset(out[0], 0, size);
        }

        for (uint32_t i = 0; i < size; ++i)
        {
            self->_buffer.write(reinterpret_cast<const char *>(&out[0][i]), sizeof(uint8_t));
        }

        self->_push(self->_micName.c_str(), self->_buffer.str());
        self->_buffer.str(std::string());
    }

    void SourceRecorder::updateTimerRecord(int newTimer)
    {
        _timerRecord = newTimer;
    }

    int SourceRecorder::getTimerRecord() const
    {
        return _timerRecord;
    }

    void SourceRecorder::clearMicName()
    {
        for (auto &c : _micName)
        {
            if (!isalnum(c))
                _micNameClear += '_';
            else
                _micNameClear += c;
        }
    }

    bool SourceRecorder::isActive() const
    {
        return _isActive;
    }

    void SourceRecorder::setActive(bool act)
    {
        _isActive = act;
    }

    void SourceRecorder::setPushFunc(std::function<void(const char *name, const std::string &)> func)
    {
        _push = func;
    }
}
