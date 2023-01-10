/*
** EPITECH PROJECT, 2021
** easystream_main_plugin
** File description:
** SpeechRecognition
*/

#ifndef SPEECHRECOGNITION_HPP_
#define SPEECHRECOGNITION_HPP_

#include "../../Common.hpp"
namespace es::obs {


    typedef struct {
        uint8_t riff[4] = {'R', 'I', 'F', 'F'};
        uint32_t chunkSize = 0;
        uint8_t wave[4] = {'W', 'A', 'V', 'E'};
        uint8_t fmt[4] = {'f', 'm', 't', ' '};
        uint32_t subChunkSize = 16;
        uint16_t audioFormat = 1;
        uint16_t numOfChan = 2;
        uint32_t samplesPerSec = 16000;
        uint32_t bytesPerSec = 16000 * 2;
        uint16_t blockAlign = 2;
        uint16_t bitsPerSample = 16;
        uint8_t subchunk2ID[4] = {'d', 'a', 't', 'a'};
        uint32_t subchunk2Size;
    } wavHeader;

    class SpeechRecognition
    {
        public:
            SpeechRecognition(obs_source_t *input);
            ~SpeechRecognition();

            static void InputAudioCaptureCallback(void *priv_data, obs_source_t *, const struct audio_data *data, bool muted);

        private:


            obs_source_t *_source;
            audio_resampler_t *resampler = nullptr;
            std::string text;
            std::vector<std::string *> output;
            std::chrono::steady_clock::time_point last_caption_at;
            std::ofstream myfile;
            int bytes_per_channel;
            bool _headerWav;
    };
}

#endif /* !SPEECHRECOGNITION_HPP_ */
