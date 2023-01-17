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
    SourceRecorder::SourceRecorder(obs_source_t *input)
        : _source(input), _outFile(AUDIOPATH, std::ios::binary), _headerWav(false)
    {
        const struct audio_output_info *obs_audio = audio_output_get_info(obs_get_audio());

        if (!obs_audio || !input)
        {
            blog(LOG_INFO, "### [EASYSTREAM::SourceRecorder] - Error getting source data.");
            return;
        }

        resample_info resample_to = {48000, AUDIO_FORMAT_16BIT, SPEAKERS_STEREO};

        // blog(LOG_INFO, "### [EASYSTREAMMMMMMMMMMMMMMMMMMMMMMMMMMMM] %d, %d, %d", resample_to.samples_per_sec, resample_to.format, resample_to.speakers);
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
            // blog(LOG_INFO, "### [EASYSTREAMMMMMMMMMMMMMMMMMMMMMMMMMMMM] %d, %d, %d", obs_audio->samples_per_sec, obs_audio->format, obs_audio->speakers);
        }

        blog(LOG_INFO, "### [EASYSTREAM::SourceRecorder] - Recorder set for input: %s", obs_source_get_name(_source));
        obs_source_add_audio_capture_callback(_source, InputAudioCaptureCallback, this);
    }

    SourceRecorder::~SourceRecorder()
    {
        _outFile.close();
    }

    void SourceRecorder::InputAudioCaptureCallback(void *priv_data, obs_source_t *source, const struct audio_data *data, bool muted)
    {
        SourceRecorder *self = static_cast<SourceRecorder *>(priv_data);

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
            blog(LOG_INFO, "### [EASYSTREAM::SourceRecorder] - Failed resampling input audio data.");
            return;
        }
        size = sizeof(int16_t) * out_frames * self->_wavFile.numOfChan;
        if (muted)
        {
            memset(out[0], 0, size);
        }

        // blog(LOG_INFO, "### [SourceRecorder]--------- started");
        // blog(LOG_INFO, "### [ts is] %ld", ts_offset);

        if (!self->_headerWav)
        {
            self->_wavFile.subchunk2Size = size;
            self->_wavFile.bytesPerSec = self->_wavFile.samplesPerSec * self->_wavFile.numOfChan * (self->_wavFile.bitsPerSample / 8);
            self->_wavFile.blockAlign = self->_wavFile.numOfChan * (self->_wavFile.bitsPerSample / 8);
            self->_wavFile.chunkSize = self->_wavFile.subchunk2Size + sizeof(wav_header_t) - 8;
            self->_outFile.write(reinterpret_cast<const char *>(&self->_wavFile), sizeof(self->_wavFile));
            self->_outFile.flush();
            self->_headerWav = true;
        }
        else
        {
            self->_wavFile.subchunk2Size += size;
            self->_wavFile.chunkSize = self->_wavFile.subchunk2Size + sizeof(wav_header_t) - 8;
            auto tmp = self->_outFile.tellp();
            self->_outFile.seekp(std::ios::beg);
            self->_outFile.write(reinterpret_cast<const char *>(&self->_wavFile), sizeof(self->_wavFile));
            self->_outFile.flush();
            self->_outFile.seekp(tmp);
        }
        for (int i = 0; i < size; ++i)
        {
            self->_outFile.write(reinterpret_cast<const char *>(&out[0][i]), sizeof(uint8_t));
        }

        // blog(LOG_INFO, "### ---------[SourceRecorder] finished");
    }
}
