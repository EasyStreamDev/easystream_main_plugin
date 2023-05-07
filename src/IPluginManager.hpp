/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** PluginManager
*/

#ifndef IPLUGINMANAGER_HPP_
#define IPLUGINMANAGER_HPP_

#include <atomic>

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
        class AsioTcpServer;
    }

    namespace transcription
    {
        class TranscriptorManager;
    }

    class IPluginManager
    {
    public:
        virtual void Init(void) = 0;
        virtual void Start(void) = 0;
        virtual void Reset(void) = 0;
        virtual const std::atomic<bool> &IsRunning(void) const = 0;

    public:
        virtual area::AreaManager *GetAreaMain(void) = 0;
        virtual obs::SourceTracker *GetSourceTracker(void) = 0;
        virtual thread::ThreadPool *GetThreadPool(void) = 0;
        virtual server::AsioTcpServer *GetServer(void) = 0;
        virtual transcription::TranscriptorManager *GetTranscriptorManager(void) = 0;
    };
} // namespace es

#endif /* !IPLUGINMANAGER_HPP_ */