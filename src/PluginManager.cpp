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
        this->Reset();
    }

    void PluginManager::Init(void)
    {
        this->m_SourceTracker->init();

        this->m_AreaMain = new es::area::AreaManager();
        this->m_Server = new es::server::AsioTcpServer(
            SERVER_HOST,
            SERVER_PORT,
            this->m_SourceTracker->getAudioMap(),
            this->m_AreaMain);
    }

    void PluginManager::Start(void)
    {
        // Start asynchrounous routines
        m_ThreadPool->push(std::function(PluginManager::RunServer), this);
        m_ThreadPool->push(std::function(PluginManager::RunArea), this);
        m_ThreadPool->push(std::function(PluginManager::RunSceneSwitcherAI), nullptr);
    }

    void PluginManager::Reset(void)
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

    void PluginManager::RunServer(void *private_data)
    {
        PluginManager *pm = static_cast<PluginManager *>(private_data);

        pm->m_Server->run(nullptr);
    }

    void PluginManager::RunArea(void *private_data)
    {
        PluginManager *pm = static_cast<PluginManager *>(private_data);

        pm->m_AreaMain->run(nullptr);
    }

    void PluginManager::RunSceneSwitcherAI(void *private_data)
    {
        es::obs::scene_switcher_ai::run(nullptr);
    }
}