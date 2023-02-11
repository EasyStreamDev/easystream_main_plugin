/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** PluginManager
*/

#ifndef IPLUGINMANAGER_HPP_
#define IPLUGINMANAGER_HPP_

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

    class IPluginManager
    {
    public:
        virtual void Init(void) = 0;
        virtual void Start(void) = 0;
        virtual void Reset(void) = 0;
        virtual const bool IsRunning(void) const = 0;

    public:
        inline virtual area::AreaManager *GetAreaMain(void) = 0;
        inline virtual obs::SourceTracker *GetSourceTracker(void) = 0;
        inline virtual thread::ThreadPool *GetThreadPool(void) = 0;
    };
} // namespace es

#endif /* !IPLUGINMANAGER_HPP_ */