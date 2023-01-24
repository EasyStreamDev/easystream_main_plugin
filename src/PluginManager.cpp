/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** PluginManager
*/

#include "PluginManager.hpp"

namespace es
{
    PluginManager::PluginManager()
    {
        this->m_SourceTracker = new es::obs::SourceTracker();
        this->m_ThreadPool = new es::thread::ThreadPool(MAX_THREAD_NUMBER);
        this->m_AreaMain = new es::area::AreaManager();
        // this->m_Server = new es::server::AsioTcpServer(
        //     es::LOCALHOST,
        //     es::PORT,
        // );
    }

    PluginManager::~PluginManager()
    {
        delete this->m_ThreadPool;
    }

    void PluginManager::Start(void)
    {
    }
}