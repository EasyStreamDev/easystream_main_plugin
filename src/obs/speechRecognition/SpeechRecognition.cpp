/*
** EPITECH PROJECT, 2021
** easystream_main_plugin
** File description:
** SpeechRecognition
*/

#include "SpeechRecognition.hpp"
#include <obs.h>

es::obs::SpeechRecognition::SpeechRecognition(obs_source_t *input): _source(input), last_caption_at(std::chrono::steady_clock::now()), myfile("C:/Users/Cedani/Documents/EasyStreamTest/easystream_main_plugin/siuuuuu.wav", std::ios::binary), _headerWav(false)
{
    
    const struct audio_output_info *obs_audio = audio_output_get_info(obs_get_audio());
    if (!obs_audio || !input) {
        blog(LOG_INFO, "[es::Obs::SpeechRecognition] Error");
        return;
    }
    // myfile.open("/home/cedani/Epitech/EIP/easyTest/speechTest/easystream_main_plugin/file.wav", std::ios::binary);

    resample_info resample_to = {48000, AUDIO_FORMAT_16BIT, SPEAKERS_STEREO};
    bytes_per_channel = get_audio_bytes_per_channel(obs_audio->format);

    blog(LOG_INFO, "[EASYSTREAMMMMMMMMMMMMMMMMMMMMMMMMMMMM] %d, %d, %d", resample_to.samples_per_sec, resample_to.format, resample_to.speakers);
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
    // std::string *str = nullptr;
    // std::string *str2 = nullptr;

    // if (!self->resampler) {
        uint8_t *out[MAX_AV_PLANES];
        // memset(out, 0, sizeof(out));

        uint32_t out_frames;
        uint64_t ts_offset;
        // blog(LOG_INFO, "[SIZE OUTPUT SPEECHRECOGNITION] pas de resampler");   
        bool success = audio_resampler_resample(self->resampler, out, &out_frames, &ts_offset, (const uint8_t *const *) data->data, data->frames);
        if (!success || !out[0]) {
            blog(LOG_INFO, "failed resampling audio data");
            return;
        }
        size = sizeof(int16_t) * out_frames * self->_wavFile.numOfChan;
    if (muted) {

        memset(out[0], 0, size);
    }
        // return;
        // str = new std::string((char *)out[0], size);
    // } else {
        // size = data->frames * get_audio_bytes_per_channel((audio_format)self->bytes_per_channel);
        // str = new std::string((char *)data->data[0], size);
        // str2 = new std::string((char *)data->data[1], size);
    // }
    // std::size_t t = get_audio_size(AUDIO_FORMAT_16BIT, SPEAKERS_STEREO, data->frames);
    // blog(LOG_INFO, "oui monsieurrrrrrrrrrrrrrrrrrrrrrrrrrrrr%ld and tssssssssssssssssssssss%ldetchannnnnnnnnnnnnnnnnnnnnnnneeeeeeeeeeeeeeel%ldetaudiosizzzzzzzzzzzzzzzzzzzzzzzzzzzze%ld", data->frames, data->timestamp, get_audio_bytes_per_channel((audio_format)self->bytes_per_channel), t);
    // self->output.push_back(str);
    // self->output.push_back(str2);
    // double secs_since_last_caption = std::chrono::duration_cast<std::chrono::duration<double >>(
    //         std::chrono::steady_clock::now() - self->last_caption_at).count();
    // if (secs_since_last_caption >= 5) {
        // wavHeader self->_wavFile;
        // obs_raw
        blog(LOG_INFO, "[es::Obs::SpeechRecognition]--------- started");
        blog(LOG_INFO, "[ts is] %ld", ts_offset);
        // self->last_caption_at = std::chrono::steady_clock::now();
        // int s = 0;
        // for (auto &n: self->output)
        //     s += n->size();
        // auto pos1 = self->myfile.tellp();
        // self->myfile << reinterpret_cast<const char *>(&self->_wavFile);
        if (!self->_headerWav) {
            self->_wavFile.subchunk2Size = size;
            self->_wavFile.bytesPerSec = self->_wavFile.samplesPerSec * self->_wavFile.numOfChan * (self->_wavFile.bitsPerSample / 8);
            self->_wavFile.blockAlign = self->_wavFile.numOfChan * (self->_wavFile.bitsPerSample / 8);
            self->_wavFile.chunkSize = self->_wavFile.subchunk2Size + sizeof(wavHeader) - 8;
            self->myfile.write(reinterpret_cast<const char *>(&self->_wavFile), sizeof(self->_wavFile));
            self->myfile.flush();
            self->_headerWav = true;
            // blog(LOG_INFO, "SHUTUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUP");
        } else {
            self->_wavFile.subchunk2Size += size;
            self->_wavFile.chunkSize = self->_wavFile.subchunk2Size + sizeof(wavHeader) - 8;
            auto tmp = self->myfile.tellp();
            self->myfile.seekp(std::ios::beg);
            self->myfile.write(reinterpret_cast<const char *>(&self->_wavFile), sizeof(self->_wavFile));
            self->myfile.flush();
            self->myfile.seekp(tmp);
        }
        // auto pos2 = self->myfile.tellp();    
        // blog(LOG_INFO, "[es::Obs::SpeechRecognition] %ld", pos2 - pos1);
        // self->myfile << std::hex << s << "\r\n";
        // for (std::size_t i = 0; i < 2; ++i) {
            // if (!data->data[i]){
            //     blog(LOG_INFO, "tartineeeeeeeeeeeeeeeeeeeeee est offeeeeeeeeeeeeeeeeeeeeeeeeeeeeer %ld", i);
            // } else {
            // uint8_t *chan1 = new uint8_t(size);
            // memcpy(chan1, data->data[0], size);
            // if (chan1)
            // int16_t *testMem = (int16_t *)malloc(sizeof(int16_t) * data->frames);
            // int16_t *testMem2 = (int16_t *)malloc(sizeof(int16_t) * data->frames);
            // memcpy(testMem, (int16_t *)data->data[0], data->frames);
            // memcpy(testMem2, (int16_t *)data->data[1], data->frames);
            // char *testMem2 = (char *)malloc(sizeof(char) * size);
            // memset(testMem, 0, size);
            // memset(testMem2, 0, size);
            // memcpy(testMem2, (char *)data->data[1], size);
            // blog(LOG_INFO, "oyeeeeeeeeeee (%ld=====%ld) oyeeeeeeeeeeeeeee [[[%s]]]", strlen(testMem), size, testMem);
            for (int i = 0; i < size; ++i) {
                // int16_t testSonTuConnais = 0;
                // testMem[i];
                // blog(LOG_INFO, "testmemiiiiiiiiiiiiiiiiiiiiii %ld", i);
                // if ((int16_t)data->data[0][i] < 0)
                //     self->myfile.write(reinterpret_cast<const char *>(&testSonTuConnais), sizeof(int16_t));
                // else
                self->myfile.write(reinterpret_cast<const char *>(&out[0][i]), sizeof(uint8_t));  
                // self->myfile.flush();
            }
            // for (int i = 0; i < size; ++i) {
            //     // int16_t testSonTuConnais = 0;
            //     // testMem[i];
            //     // blog(LOG_INFO, "testmemiiiiiiiiiiiiiiiiiiiiii %ld", i);
            //     // if ((int16_t)data->data[0][i] < 0)
            //     //     self->myfile.write(reinterpret_cast<const char *>(&testSonTuConnais), sizeof(int16_t));
            //     // else
            //     self->myfile.write(reinterpret_cast<const char *>(&out[0][i]), sizeof(int16_t));  
            //     self->myfile.flush();
            // }
            blog(LOG_INFO, "---------finished first chan---------%ld", size);
            // if (out[1]) {
            //     for (int i = 0; i < out_frames; ++i) {
            //         blog(LOG_INFO, "il rentre quand meme %ld", i);
            //         // testMem[i];
            //         // if (data->data[0][i])
            //         self->myfile.write(reinterpret_cast<const char *>(&out[1][i]), sizeof(uint8_t));
            //         self->myfile.flush();
            //     }
            // }

            // self->myfile.write(reinterpret_cast<const char *>(&data->data[1][i]), sizeof(uint16_t));
            // self->myfile.flush();
            // self->myfile.write(reinterpret_cast<const char *>(testMem2[i]), sizeof(int32_t));
            // for (int i = 0; i < data->frames; ++i) {
            // //     // testMem[i];
            // //     // blog(LOG_INFO, "testmemiiiiiiiiiiiiiiiiiiiiii %ld and sizeeeeeeeeeeeiiiiiiiiiiiiiiiiiiiiiii%c", i, testMem[i]);
            // //     // self->myfile.write(&data->data[0][i], sizeof(uint16_t));
            //     self->myfile.write(reinterpret_cast<const char *>(&data->data[1][i]), sizeof(uint16_t));
            // //     // self->myfile.write(reinterpret_cast<const char *>(testMem2[i]), sizeof(int16_t));
            // }
            
                // self->myfile.write(&testMem2[i], sizeof(uint16_t));
                // self->myfile.write(testMem, sizeof(int16_t) * strlen(testMem));
                // self->myfile.write(testMem2, sizeof(int16_t) * strlen(testMem2));
            // self->myfile.write(reinterpret_cast<const char *>(data->data[1]), size * sizeof(int32_t));
            // self->myfile.write((char *)data->data[1], size);
            // self->myfile << (char *)data->data[0];
            // self->myfile << (char *)data->data[0];
            // self->myfile.flush();
            // self->myfile.write((char *)data->data[1], size);
            // self->myfile.flush();
            // self->myfile << *n;
            // }
            // self->myfile.write((char *)data->data[0], sizeof(uint8_t));
            // self->myfile.flush();
        // }
        // for (auto &n: self->output) {
        //     for (auto $c: *n) {
        //         sizeof(float);
        //     }
        //     if (self->myfile.is_open())
        //     self->myfile << (char *)n; 
        //     blog(LOG_INFO, "[es::Obs::SpeechRecognition] write waf files %ld", self->output.size());
        // };
        // auto tmp = str[0];

        // for (auto &n: self->output) {
            // for(std::size_t i = 0; i < n->size(); i += sizeof(float)) {
                // blog(LOG_INFO, "etsizeeeeeeeeeeeeeeeeeeeeeeee=%d", s);
                // blog(LOG_INFO, "easystreammmmmmmmmmmmmmmmmmmmmmmmmm mmmmm===========================%ld", i);
                // self->myfile.write((*n).substr(i, sizeof(float)).c_str(), sizeof(float));
                // self->myfile.flush();
            // }
        // }
        
        // self->myfile << "\r\n";
        // self->output.clear();
        blog(LOG_INFO, "---------[es::Obs::SpeechRecognition] finished");
    // }

    //blog(LOG_INFO, "[es::Obs::SpeechRecognition] Size: %d", size);
}