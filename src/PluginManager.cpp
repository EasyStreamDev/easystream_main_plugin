/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** PluginManager
*/

#include "PluginManager.hpp"

namespace es
{
    void test_transcription_func(void *private_data)
    {
        PluginManager *pm = static_cast<PluginManager *>(private_data);
        const std::string full_path_base = "/home/yem/delivery/Epitech/EIP/easystream_main_plugin/Tests/ressources/";
        const std::vector<std::string> paths = {
            full_path_base + "league.wav",
            full_path_base + "sweden.wav",
            full_path_base + "untitled.wav"};

        while (1)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            pm->GetTranscriptorManager()->submit(paths[rand() % paths.size()]);
        }
    }
}

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
        m_Running = true;

        this->m_SourceTracker->init();

        this->m_TranscriptorManager = new es::transcription::TranscriptorManager();
        this->m_AreaMain = new es::area::AreaManager();
        this->m_Server = new es::server::AsioTcpServer(SERVER_HOST, SERVER_PORT, this);
    }

    void PluginManager::Start(void)
    {
        // Start asynchrounous routines
        m_ThreadPool->push(std::function(PluginManager::RunServer), this);
        m_ThreadPool->push(std::function(PluginManager::RunArea), this);
        m_ThreadPool->push(std::function(PluginManager::RunSceneSwitcherAI), nullptr);
        m_ThreadPool->push(std::function(PluginManager::RunTranscriptor), this);
        m_ThreadPool->push(std::function(test_transcription_func), this);
    }

    void PluginManager::Reset(void)
    {
        m_Running = false;
        { // Module deletion commented (probably crashing)
          // blog(LOG_INFO, "\n### --- DELETING : AREA Main.");
          // if (this->m_AreaMain)
          // {
          //     delete this->m_AreaMain;
          // }
          // blog(LOG_INFO, "\n### --- DELETED : AREA Main.");
          // blog(LOG_INFO, "\n### --- DELETING : Server.");
          // if (this->m_Server)
          // {
          //     delete this->m_Server;
          // }
          // blog(LOG_INFO, "\n### --- DELETED : Server.");
          // blog(LOG_INFO, "\n### --- DELETING : Source tracker.");
          // if (this->m_SourceTracker)
          // {
          //     delete this->m_SourceTracker;
          // }
          // blog(LOG_INFO, "\n### --- DELETED : Source tracker.");
          // blog(LOG_INFO, "\n### --- DELETING : ThreadPool.");
          // if (this->m_ThreadPool)
          // {
          //     delete this->m_ThreadPool;
          // }
          // blog(LOG_INFO, "\n### --- DELETED : ThreadPool.");
        }
    }

    const std::atomic<bool> &PluginManager::IsRunning(void) const
    {
        return m_Running;
    }

    area::AreaManager *PluginManager::GetAreaMain(void)
    {
        return m_AreaMain;
    }

    server::AsioTcpServer *PluginManager::GetServer(void)
    {
        return m_Server;
    }

    obs::SourceTracker *PluginManager::GetSourceTracker(void)
    {
        return m_SourceTracker;
    }

    thread::ThreadPool *PluginManager::GetThreadPool(void)
    {
        return m_ThreadPool;
    }

    transcription::TranscriptorManager *PluginManager::GetTranscriptorManager(void)
    {
        return m_TranscriptorManager;
    }

    /**************************/
    /* ASYNCHROUNOUS ROUTINES */
    /**************************/

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

    void PluginManager::RunTranscriptor(void *private_data)
    {
        PluginManager *pm = static_cast<PluginManager *>(private_data);

        pm->m_TranscriptorManager->run(nullptr);
    }
}