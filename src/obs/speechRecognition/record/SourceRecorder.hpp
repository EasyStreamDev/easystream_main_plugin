/*
** EPITECH PROJECT, 2021
** easystream_main_plugin
** File description:
** SourceRecorder
*/

#ifndef SOURCERECORDER_HPP_
#define SOURCERECORDER_HPP_

#include "../../../Common.hpp"
#include "Variables.h"
#include <sstream>
#define TIMER_RECORD 5
#include <filesystem>
#include <functional>
#include <atomic>
namespace es::obs
{
    // 1597476

    typedef struct wav_header_s
    {
        uint8_t riff[4] = {'R', 'I', 'F', 'F'};
        uint32_t chunkSize = 0;
        uint8_t wave[4] = {'W', 'A', 'V', 'E'};
        uint8_t fmt[4] = {'f', 'm', 't', ' '};
        uint32_t subChunkSize = 16;
        uint16_t audioFormat = 1;
        uint16_t numOfChan = 2;
        uint32_t samplesPerSec = 48000;
        uint32_t bytesPerSec = 0;
        uint16_t blockAlign = 0;
        uint16_t bitsPerSample = 16;
        uint8_t subchunk2ID[4] = {'d', 'a', 't', 'a'};
        uint32_t subchunk2Size = 0;
    } wav_header_t;

    class SourceRecorder
    {
    public:
        SourceRecorder();
        ~SourceRecorder();

        static void InputAudioCaptureCallback(void *priv_data, obs_source_t *, const struct audio_data *data, bool muted);
        void run(void *);
        void updateTimerRecord(int newTimer);
        int getTimerRecord() const;
        bool isActive() const;
        void setActive(bool);
        void setPushFunc(std::function<void (const char *name, const std::string &)>);
        void init(obs_source_t *input, const std::string & micName); 

    private:
        std::filesystem::path _temporaryPath;
        obs_source_t *_source;
        audio_resampler_t *_resampler = nullptr;
        std::ofstream _outFile;
        bool _headerWav;
        wav_header_t _wavFile;
        std::stringstream _buffer;
        // int _sizeQueue = 0;
        // std::chrono::steady_clock::time_point _checkPoint;
        // std::function<uint (const std::string &)> _submitFile;
        int nbOutNb = 0;
        std::string _micName;
        std::string _micNameClear;
        std::atomic_int _timerRecord;
        std::atomic_bool _isActive;
        std::function<void (const char *, const std::string &)> _push;

        


        void clearMicName();
    };
}

#endif /* !SOURCERECORDER_HPP_ */
