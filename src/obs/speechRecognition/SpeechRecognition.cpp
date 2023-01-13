/*
** EPITECH PROJECT, 2021
** easystream_main_plugin
** File description:
** SpeechRecognition
*/

#include "SpeechRecognition.hpp"
#include <obs.h>

es::obs::SpeechRecognition::SpeechRecognition(obs_source_t *input) : _source(input), last_caption_at(std::chrono::steady_clock::now()), myfile(AUDIOPATH, std::ios::binary), _headerWav(false)
{

    const struct audio_output_info *obs_audio = audio_output_get_info(obs_get_audio());
    if (!obs_audio || !input)
    {
        blog(LOG_INFO, "[es::Obs::SpeechRecognition] Error");
        return;
    }
    // myfile.open("/home/cedani/Epitech/EIP/easyTest/speechTest/easystream_main_plugin/file.wav", std::ios::binary);

    resample_info resample_to = {48000, AUDIO_FORMAT_16BIT, SPEAKERS_STEREO};
    bytes_per_channel = get_audio_bytes_per_channel(obs_audio->format);

    blog(LOG_INFO, "[EASYSTREAMMMMMMMMMMMMMMMMMMMMMMMMMMMM] %d, %d, %d", resample_to.samples_per_sec, resample_to.format, resample_to.speakers);
    if (obs_audio->samples_per_sec != resample_to.samples_per_sec || obs_audio->format != resample_to.format || obs_audio->speakers != resample_to.speakers)
    {
        resample_info src = {
            obs_audio->samples_per_sec,
            obs_audio->format,
            obs_audio->speakers};
        resampler = audio_resampler_create(&resample_to, &src);
        if (!resampler)
            throw std::string("Failed to create audio resampler");
        blog(LOG_INFO, "[EASYSTREAMMMMMMMMMMMMMMMMMMMMMMMMMMMM] %d, %d, %d", obs_audio->samples_per_sec, obs_audio->format, obs_audio->speakers);
    }

    blog(LOG_INFO, "[es::Obs::SpeechRecognition] Speech recognition for input: %s", obs_source_get_name(_source));
    obs_source_add_audio_capture_callback(_source, InputAudioCaptureCallback, this);
}

es::obs::SpeechRecognition::~SpeechRecognition()
{
    myfile.close();
}

void es::obs::SpeechRecognition::InputAudioCaptureCallback(void *priv_data, obs_source_t *source, const struct audio_data *data, bool muted)
{
    SpeechRecognition *self = static_cast<es::obs::SpeechRecognition *>(priv_data);

    if (!data || !data->frames || !self)
        return;

    unsigned int size = 0;
    uint8_t *out[MAX_AV_PLANES];
    uint32_t out_frames;
    uint64_t ts_offset;
    bool success = audio_resampler_resample(self->resampler, out, &out_frames, &ts_offset, (const uint8_t *const *)data->data, data->frames);
    if (!success || !out[0])
    {
        blog(LOG_INFO, "failed resampling audio data");
        return;
    }
    size = sizeof(int16_t) * out_frames * self->_wavFile.numOfChan;
    if (muted)
    {
        memset(out[0], 0, size);
    }
    // double secs_since_last_caption = std::chrono::duration_cast<std::chrono::duration<double >>(
    //         std::chrono::steady_clock::now() - self->last_caption_at).count();
    // if (secs_since_last_caption >= 5) {
    // obs_raw
    blog(LOG_INFO, "[es::Obs::SpeechRecognition]--------- started");
    blog(LOG_INFO, "[ts is] %ld", ts_offset);
    // self->last_caption_at = std::chrono::steady_clock::now();
    if (!self->_headerWav)
    {
        self->_wavFile.subchunk2Size = size;
        self->_wavFile.bytesPerSec = self->_wavFile.samplesPerSec * self->_wavFile.numOfChan * (self->_wavFile.bitsPerSample / 8);
        self->_wavFile.blockAlign = self->_wavFile.numOfChan * (self->_wavFile.bitsPerSample / 8);
        self->_wavFile.chunkSize = self->_wavFile.subchunk2Size + sizeof(wavHeader) - 8;
        self->myfile.write(reinterpret_cast<const char *>(&self->_wavFile), sizeof(self->_wavFile));
        self->myfile.flush();
        self->_headerWav = true;
    }
    else
    {
        self->_wavFile.subchunk2Size += size;
        self->_wavFile.chunkSize = self->_wavFile.subchunk2Size + sizeof(wavHeader) - 8;
        auto tmp = self->myfile.tellp();
        self->myfile.seekp(std::ios::beg);
        self->myfile.write(reinterpret_cast<const char *>(&self->_wavFile), sizeof(self->_wavFile));
        self->myfile.flush();
        self->myfile.seekp(tmp);
    }
    for (int i = 0; i < size; ++i)
        self->myfile.write(reinterpret_cast<const char *>(&out[0][i]), sizeof(uint8_t));
    blog(LOG_INFO, "---------[es::Obs::SpeechRecognition] finished");
}