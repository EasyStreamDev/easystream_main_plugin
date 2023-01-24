/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** PluginManager
*/

#ifndef PLUGINMANAGER_HPP_
#define PLUGINMANAGER_HPP_

#include "obs/SourceTracker.hpp"
#include "area/AreaManager.hpp"
#include "server/include/AsioTcpServer.hpp"

namespace es
{
    const size_t MAX_THREAD_NUMBER = 10;
    const std::string LOCALHOST = "0.0.0.0";
    const int PORT = 47920;

    class PluginManager
    {
    public:
        PluginManager();
        ~PluginManager();

        void Start(void);

    public:
        inline area::AreaManager *GetAreaMain(void) { return m_AreaMain; }
        // inline server::AsioTcpServer *GetServer(void) { return m_Server; }
        inline obs::SourceTracker *GetSourceTracker(void) { return m_SourceTracker; }
        inline thread::ThreadPool *GetThreadPool(void) { return m_ThreadPool; }

    private:
        area::AreaManager *m_AreaMain;
        server::AsioTcpServer *m_Server;
        obs::SourceTracker *m_SourceTracker;
        thread::ThreadPool *m_ThreadPool;
    };
} // namespace es

#endif /* !PLUGINMANAGER_HPP_ */