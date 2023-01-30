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

        void Init(void);
        void Start(void);

    public:
        inline area::AreaManager *GetAreaMain(void) { return m_AreaMain; }
        // inline server::AsioTcpServer *GetServer(void) { return m_Server; } // @note maybe not necessary
        inline obs::SourceTracker *GetSourceTracker(void) { return m_SourceTracker; }
        inline thread::ThreadPool *GetThreadPool(void) { return m_ThreadPool; }

    private:
        // inline static void RunServer(std::shared_ptr<void>){m_Server->run();};

    private:
        thread::ThreadPool *m_ThreadPool = nullptr;
        obs::SourceTracker *m_SourceTracker = nullptr; // @dev : should auto-leveler be separate runnable ?

        // Runnable
        area::AreaManager *m_AreaMain = nullptr;   // Runnable done
        server::AsioTcpServer *m_Server = nullptr; // Runnable done
    };
} // namespace es

#endif /* !PLUGINMANAGER_HPP_ */