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
    SourceRecorder::SourceRecorder(obs_source_t *input, const std::function<uint(const std::string &)> &func, const std::string &micName, size_t timerRecord)
        : _temporaryPath(std::filesystem::temp_directory_path()), _source(input), _headerWav(false), _checkPoint(std::chrono::steady_clock::now()), _submitFile(func), _micName(micName), _timerRecord(timerRecord)
    {
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
        resample_info resample_to = {48000, AUDIO_FORMAT_16BIT, SPEAKERS_STEREO};

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

        blog(LOG_INFO, "### [SourceRecorder] - Recorder set for input: %s", obs_source_get_name(_source));
    }

    SourceRecorder::~SourceRecorder()
    {
        _outFile.close();
        std::filesystem::remove_all(_temporaryPath);
    }

    void SourceRecorder::run(void *)
    {
        // std::cout << _micName << " BEFORE        RECORDING            " << std::endl;
        obs_source_add_audio_capture_callback(_source, InputAudioCaptureCallback, this);
        // std::cout << _micName << " STARTTTINNNNNNNNNNNNNNG        RECORDING            " << std::endl;
        while (1)
        {
            /* code */
        };
    }

    void SourceRecorder::InputAudioCaptureCallback(void *priv_data, obs_source_t *source, const struct audio_data *data, bool muted)
    {
        using namespace std::chrono_literals;
        SourceRecorder *self = static_cast<SourceRecorder *>(priv_data);

        if (!data || !data->frames || !self)
        {
            return;
        }

        // std::cout << "Storing data " << self->_micName << std::endl;

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

        // blog(LOG_INFO, "### [SourceRecorder]--------- started");
        // blog(LOG_INFO, "### [ts is] %ld", ts_offset);

        if (!self->_headerWav)
        {
            self->_wavFile.subchunk2Size = size;
            self->_wavFile.bytesPerSec = self->_wavFile.samplesPerSec * self->_wavFile.numOfChan * (self->_wavFile.bitsPerSample / 8);
            self->_wavFile.blockAlign = self->_wavFile.numOfChan * (self->_wavFile.bitsPerSample / 8);
            self->_wavFile.chunkSize = self->_wavFile.subchunk2Size + sizeof(wav_header_t) - 8;
            self->_buffer.write(reinterpret_cast<const char *>(&self->_wavFile), sizeof(self->_wavFile));
            self->_buffer.flush();
            self->_headerWav = true;
        }
        else
        {
            self->_wavFile.subchunk2Size += size;
            self->_wavFile.chunkSize = self->_wavFile.subchunk2Size + sizeof(wav_header_t) - 8;
            auto tmp = self->_buffer.tellp();
            self->_buffer.seekp(std::ios::beg);
            self->_buffer.write(reinterpret_cast<const char *>(&self->_wavFile), sizeof(self->_wavFile));
            self->_buffer.flush();
            self->_buffer.seekp(tmp);
        }
        for (int i = 0; i < size; ++i)
        {
            // self->_outFile.write(reinterpret_cast<const char *>(&out[0][i]), sizeof(uint8_t));
            self->_buffer.write(reinterpret_cast<const char *>(&out[0][i]), sizeof(uint8_t));
        }

        std::chrono::duration<float> timer = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - self->_checkPoint);
        if (timer.count() >= self->_timerRecord)
        {
            // // for test
            // self->_outFile.open(self->_temporaryPath.string() + "/output" + std::to_string(self->nbOutNb++) + ".wav", std::ios::binary);
            // // self->_outFile.clear();
            // self->_outFile << self->_buffer.str();
            // self->_outFile.close();

            // std::cout << "Storing data " << self->_temporaryPath.string() + "/" + self->_micNameClear << std::endl;

            // for transcriptor
            self->_outFile.open(self->_temporaryPath.string() + "/" + self->_micNameClear + ".wav", std::ios::binary);
            // if (!self->_outFile.is_open())
            //     std::cout << "cannot open" << std::endl;
            // self->_outFile.clear();
            self->_outFile << self->_buffer.str();
            self->_outFile.close();

            self->_submitFile(self->_temporaryPath.string() + self->_micName);
            // self->_buffer.clear();
            self->_buffer.str(std::string());
            self->_checkPoint = std::chrono::steady_clock::now();
        }
        // blog(LOG_INFO, "### ---------[SourceRecorder] finished");
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
}
