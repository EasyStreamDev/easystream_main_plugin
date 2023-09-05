/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** PluginManager
*/

#ifndef IPLUGINMANAGER_HPP_
#define IPLUGINMANAGER_HPP_

#include <atomic>
#include <iostream>
#include "utils/Json.hpp"

namespace es
{
    namespace area
    {
        class AreaManager;
    }

    namespace obs
    {
        class SourceTracker;
    }

    namespace thread
    {
        class ThreadPool;
    }

    namespace server
    {
        class IServer;
        class AsioTcpServer;
    }

    namespace transcript
    {
        class Transcriptor;
    }

    namespace subtitles
    {
        class SubtitlesManager;
    }

    class IPluginManager
    {
    public:
        virtual void Init(void) = 0;
        virtual void Start(void) = 0;
        virtual void Stop(void) = 0;
        virtual const std::atomic<bool> &IsRunning(void) const = 0;

    public:
        virtual area::AreaManager *GetAreaMain(void) = 0;
        virtual obs::SourceTracker *GetSourceTracker(void) = 0;
        virtual thread::ThreadPool *GetThreadPool(void) = 0;
        virtual server::IServer *GetServer(void) = 0;
        virtual subtitles::SubtitlesManager *GetSubtitlesManager(void) = 0;
        virtual transcript::Transcriptor *GetTranscriptor(void) = 0;


        //
        virtual int addRecorder(const std::string micName) = 0;
        virtual bool changeTimer(std::string micName, int newTimer) = 0;
        virtual json getAllRecorders() = 0;
    };
} // namespace es

#endif /* !IPLUGINMANAGER_HPP_ */