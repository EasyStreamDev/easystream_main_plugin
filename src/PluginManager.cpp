/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** PluginManager
*/

#include "PluginManager.hpp"

namespace es::testing
{
    void test_user_profile(void *private_data)
    {
        user::UserProfile up;

        while (1)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5 * 1000));
            std::cerr << up.getData().dump(4) << std::endl;
        }
    }
}

namespace es
{
    PluginManager::PluginManager()
        : m_AreaManager(new es::area::AreaManager()),
          m_Server(new es::server::AsioTcpServer(SERVER_HOST, SERVER_PORT, this))
    {
        this->m_SourceTracker = new es::obs::SourceTracker();
        this->m_ThreadPool = new es::thread::ThreadPool(MAX_THREAD_NUMBER);
        this->m_SubtitlesManager = new es::subtitles::SubtitlesManager();
        _transcriptor = new es::transcript::Transcriptor();
    }

    PluginManager::~PluginManager()
    {
#ifdef unix
        kill(_pyProgramPid, SIGQUIT);
#endif
        this->Stop();
    }

    void PluginManager::Init(void)
    {
        m_Running = true;

        this->m_SourceTracker->init(this);

        // this->m_TranscriptorManager = new es::transcription::TranscriptorManager();
        // this->m_AreaManager = new es::area::AreaManager();
        // this->m_Server = new es::server::AsioTcpServer(SERVER_HOST, SERVER_PORT, this);
    }

    void PluginManager::Start(void)
    {
        // Start asynchrounous routines
        m_ThreadPool->push(std::function(PluginManager::RunServer), this);
        m_ThreadPool->push(std::function(PluginManager::RunArea), this);
        // m_ThreadPool->push(std::function(PluginManager::RunRecorder), this);
        m_ThreadPool->push(std::function(PluginManager::RunTranscriptor), this);
        // m_ThreadPool->push(std::function(PluginManager::RunSceneSwitcherAI), nullptr);
        m_ThreadPool->push(std::function(PluginManager::RunSubTitles), this);
        // m_ThreadPool->push(std::function(PluginManager::RunPyProgram), this);

        { // Testing functions
            m_ThreadPool->push(std::function(testing::test_user_profile), this);
            // m_ThreadPool->push(std::function(testing::test_transcription_submit), this);
            // m_ThreadPool->push(std::function(testing::test_transcription_results), this);
        }
    }

    void PluginManager::Stop(void)
    {
        m_Running = false;
        { // Module deletion commented (probably crashing)
          // blog(LOG_INFO, "\n### --- DELETING : AREA Main.");
          // if (this->m_AreaManager)
          // {
          //     delete this->m_AreaManager;
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
        return m_AreaManager;
    }

    server::IServer *PluginManager::GetServer(void)
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

    // transcription::TranscriptorManager *PluginManager::GetTranscriptorManager(void)
    // {
    //     return m_TranscriptorManager;
    // }

    transcript::Transcriptor *PluginManager::GetTranscriptor(void)
    {
        return _transcriptor;
    }

    subtitles::SubtitlesManager *PluginManager::GetSubtitlesManager(void)
    {
        return m_SubtitlesManager;
    }

    /**************************/
    /* ASYNCHROUNOUS ROUTINES */
    /**************************/

    void PluginManager::RunServer(void *private_data)
    {
        PluginManager *pm = static_cast<PluginManager *>(private_data);

        pm->m_Server.load()->run(nullptr);
    }

    void PluginManager::RunArea(void *private_data)
    {
        PluginManager *pm = static_cast<PluginManager *>(private_data);

        pm->m_AreaManager.load()->run(nullptr);
    }

    void PluginManager::RunSceneSwitcherAI(void *private_data)
    {
        es::obs::scene_switcher_ai::run(nullptr);
    }

    void PluginManager::RunSubTitles(void *private_data)
    {
        PluginManager *pm = static_cast<PluginManager *>(private_data);

        pm->m_SubtitlesManager.load()->run(pm);
    }

    void PluginManager::RunTranscriptor(void *private_data)
    {
        PluginManager *pm = static_cast<PluginManager *>(private_data);
        transcript::Transcriptor *tm = pm->_transcriptor;
        // // transcription::TranscriptorManager *tm = pm->m_TranscriptorManager.load();

        tm->run(pm);
        // tm->init(pm);
        // tm->run(nullptr);
    }

    void PluginManager::RunRecorder(void *private_data)
    {
        // using namespace std::chrono_literals;
        // std::this_thread::sleep_for(2s);
        // PluginManager *pm = static_cast<PluginManager *>(private_data);
        // obs_source_t *source = obs_get_source_by_name("Mic/Aux");
        // pm->_recorders["Mic/Aux"] = new obs::SourceRecorder(source, [pm](const std::string &fp) -> uint
        //                                         { return 1; });
        // pm->_recorders["Mic/Aux"]->run(nullptr);
    }

    void PluginManager::RunPyProgram(void *private_data)
    {
        PluginManager *pm = static_cast<PluginManager *>(private_data);

#ifdef unix
        pm->_pyProgramPid = fork();

        if (pm->_pyProgramPid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pm->_pyProgramPid > 0)
        {
            std::cout << "running obs (parent process)" << std::endl;
        }
        else
        {
            char *args[] = {strdup(TRANSCRIPTPATH), NULL};
            execv(args[0], args);
        }
#endif
    }

    int PluginManager::addRecorder(const std::string micName)
    {
        // if (_recorders.find(micName) != _recorders.end())
        //     return -1;
        // obs_source_t *source = obs_get_source_by_name(micName.c_str());
        // std::cout << "Add Recorder " << micName << std::endl;
        // if (!source)
        //     return -2;
        // _recorders[micName] = new obs::SourceRecorder(source, [this](const std::string &fp) -> uint
        //                                         { return 1; }, micName);
        // m_ThreadPool->push([this, micName](void *) {
        //     std::cout << "Ok here : " << micName << std::endl;
        //     this->_recorders[std::string(micName)]->run(nullptr);
        // }, nullptr);
        return 1;
    }

    bool PluginManager::changeTimer(std::string micName, int newTimer)
    {
        if (_recorders.find(micName) != _recorders.end())
        {
            _recorders[micName]->updateTimerRecord(newTimer);
            return true;
        }
        return false;
    }

    json PluginManager::getAllRecorders()
    {
        json result;
        result["data"] = {};
        for (const auto &rec : _recorders)
        {
            result["data"] += json{
                {"micName", rec.first},
                {"offset", rec.second->getTimerRecord()}};
        }
        return result;
    }
}