#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <unordered_map>
#include <queue>
#include "../../Runnable.hpp"
#include "record/SourceRecorder.hpp"
#include "DataStream/DataStream.hpp"
#include "TcpClient/AsioClient.hpp"
#include <string>

namespace es
{
    class PluginManager;
}

namespace es::transcript
{
    struct micsInfo {
        bool _isEnableWordDetection = false;
        obs::SourceRecorder *_recorder = nullptr;
        int _nbTextFieldLinked = 0;
        DataStream *_pusher = nullptr;
    };

    class Transcriptor : public Runnable {
        public:
            Transcriptor();
            ~Transcriptor();

            void enableSubtitlesOnMic(const char *micName, const std::string &language = "none");
            void disableSubtitlesOnMic(std::string);
            // void enableWordDetectOnMic(const char *micName);
            // void disableWordDetcctOnMic(const char*micName);
            // void startNewRecorderByName(const char *micName);
            json getAllActiveMics();
            void pushAudio(const char *name, const std::string &);
            // void setFuncSubmitWordsArea(std::function<void (const char *)>);
            // void setFuncSubmitWordsSubtitles(std::function<void (const char *)>);
            void stop();
            void run(void *);
        private:
            // void createNewTranscription(int idClient);

            void enableMics(json);
            void publishTranscript(json);

            void executeRequest();

            std::atomic_bool _isRunning;
            std::function<void (const char *)> _submitWordsArea;
            std::function<void (const char *)> _submitWordsSubtitles;
            std::mutex _mtx;
            std::mutex _mtxPush;
            std::condition_variable _cVar;
            std::unordered_map<std::string, micsInfo *> _recorders;
            // Tcp client
            AsioClient _tcpCli;
            std::queue<json> _tasks;
            // Threads vector
            std::vector<std::thread> _threads;
            
            //pluginmanager
            PluginManager *_pluginManager = nullptr;
    };
}


//
//
//                              transcriptor
//
//          setSubtitle                             
//
//
//
//
//
//
//