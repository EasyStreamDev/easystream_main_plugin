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
    }

    PluginManager::~PluginManager()
    {
        if (this->m_AreaMain)
        {
            delete this->m_AreaMain;
        }
        if (this->m_Server)
        {
            delete this->m_Server;
        }
        if (this->m_ThreadPool)
        {
            delete this->m_ThreadPool;
        }
        if (this->m_SourceTracker)
        {
            delete this->m_SourceTracker;
        }
    }

    void PluginManager::Init(void)
    {
        this->m_SourceTracker->init();

        this->m_AreaMain = new es::area::AreaManager();
        this->m_Server = new es::server::AsioTcpServer(
            es::LOCALHOST,
            es::PORT,
            this->m_SourceTracker->getAudioMap(),
            this->m_AreaMain);
    }

    void PluginManager::Start(void)
    {
        // @todo : run all runnables
        // this->m_ThreadPool->push(std::function(m_Server->run), nullptr);
        // this->m_ThreadPool->push(std::function(m_AreaMain->run), nullptr);
    }
}