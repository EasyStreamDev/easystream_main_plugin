/*
** EPITECH PROJECT, 2021
** easystream_main_plugin
** File description:
** SpeechRecognition
*/

#include "SpeechRecognition.hpp"
#include <obs.h>

es::obs::SpeechRecognition::SpeechRecognition(obs_source_t *input): _source(input), last_caption_at(std::chrono::steady_clock::now()), myfile("/home/cedani/Epitech/EIP/easyTest/speechTest/easystream_main_plugin/file.wav", std::ios::binary | std::ios::out), _headerWav(false)
{
    const struct audio_output_info *obs_audio = audio_output_get_info(obs_get_audio());
    if (!obs_audio || !input) {
        blog(LOG_INFO, "[es::Obs::SpeechRecognition] Error");
        return;
    }
    // myfile.open("/home/cedani/Epitech/EIP/easyTest/speechTest/easystream_main_plugin/file.wav", std::ios::binary);

    resample_info resample_to = {16000, AUDIO_FORMAT_16BIT, SPEAKERS_MONO};
    bytes_per_channel = get_audio_bytes_per_channel(resample_to.format);

    if (obs_audio->samples_per_sec != resample_to.samples_per_sec
        || obs_audio->format != resample_to.format
        || obs_audio->speakers != resample_to.speakers) {
        resample_info src = {
                obs_audio->samples_per_sec,
                obs_audio->format,
                obs_audio->speakers
        };

        resampler = audio_resampler_create(&resample_to, &src);
        if (!resampler)
            throw std::string("Failed to create audio resampler");
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

    if (muted)
        return;
    if (!data || !data->frames)
        return;

    unsigned int size = 0;
    std::string *str = nullptr;

    if (!self->resampler) {
        uint8_t *out[MAX_AV_PLANES];
        memset(out, 0, sizeof(out));

        uint32_t out_frames;
        uint64_t ts_offset;
        blog(LOG_INFO, "[SIZE OUTPUT SPEECHRECOGNITION] pas de resampler");   

        bool success = audio_resampler_resample(self->resampler, out, &out_frames, &ts_offset, (const uint8_t *const *) data->data, data->frames);
        if (!success || !out[0]) {
            blog(LOG_INFO, "failed resampling audio data");
            return;
        }
        size = out_frames * get_audio_bytes_per_channel((audio_format)self->bytes_per_channel);
        str = new std::string((char *)out[0], size);
    } else {
        
        size = data->frames * get_audio_bytes_per_channel((audio_format)self->bytes_per_channel);
        str = new std::string((char *)data->data[0], size);
    }
    self->output.push_back(str);
    double secs_since_last_caption = std::chrono::duration_cast<std::chrono::duration<double >>(
            std::chrono::steady_clock::now() - self->last_caption_at).count();
    if (secs_since_last_caption >= 5) {
        wavHeader wavFile;
        blog(LOG_INFO, "[es::Obs::SpeechRecognition]---------");
        self->last_caption_at = std::chrono::steady_clock::now();
        int s = 0;
        for (auto &n: self->output)
            s += n->size();
        wavFile.chunkSize += s;
        wavFile.subchunk2Size = s + sizeof(wavHeader) - 44;
        // auto pos1 = self->myfile.tellp();
        // self->myfile << reinterpret_cast<const char *>(&wavFile);
        if (!self->_headerWav) {
            self->myfile.write(reinterpret_cast<const char *>(&wavFile), sizeof(wavFile));
            self->myfile.flush();
            self->_headerWav = true;
        } else {
            auto tmp = self->myfile.tellp();
            self->myfile.seekp(std::ios::beg);
            self->myfile.write(reinterpret_cast<const char *>(&wavFile), sizeof(wavFile));
            self->myfile.flush();
            self->myfile.seekp(tmp);
        }
        // auto pos2 = self->myfile.tellp();    
        // blog(LOG_INFO, "[es::Obs::SpeechRecognition] %ld", pos2 - pos1);
        // self->myfile << std::hex << s << "\r\n";
        for (auto &n: self->output) {
            // if (self->myfile.is_open())
            //     blog(LOG_INFO, "[es::Obs::SpeechRecognition] write waf files");
            self->myfile.write(n->c_str(), sizeof(uint8_t));
            self->myfile.flush();
            // self->myfile << *n;
        }
        
        // self->myfile << "\r\n";
        self->output.clear();
        blog(LOG_INFO, "---------[es::Obs::SpeechRecognition]");
    }

    //blog(LOG_INFO, "[es::Obs::SpeechRecognition] Size: %d", size);
}