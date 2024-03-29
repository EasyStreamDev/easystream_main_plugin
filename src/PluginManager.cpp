/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** PluginManager
*/

#include "PluginManager.hpp"
#ifdef _WIN32
#include <tchar.h>
#endif
namespace es::testing
{
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
        this->m_Transcriptor = new es::transcript::Transcriptor();
        this->m_topAudioMicManager = new es::obs::TopAudioMic();
    }

    PluginManager::~PluginManager()
    {
#if BUILD_TYPE == RELEASE
#ifdef _WIN32
    CloseHandle(_piEchostra.hProcess);
    CloseHandle(_piEchostra.hThread);
#endif


#ifdef unix
        kill(_pyProgramPid, 9);
#endif
#endif
        this->Stop();
    }

    void PluginManager::Init(void)
    {
        this->m_Running = true;
        this->m_SourceTracker->init(this);
    }

    void PluginManager::Start(void)
    {
        // Start asynchrounous routines
    #if BUILD_TYPE==RELEASE_EASYSTREAM
        m_ThreadPool->push(std::function(PluginManager::RunEchostra), this);
    #endif
        m_ThreadPool->push(std::function(PluginManager::RunServer), this);
        m_ThreadPool->push(std::function(PluginManager::RunArea), this);
        m_ThreadPool->push(std::function(PluginManager::RunSubTitles), this);
        m_ThreadPool->push(std::function(PluginManager::RunTranscriptor), this);
        // m_ThreadPool->push(std::function(PluginManager::RunSceneSwitcherAI), nullptr);

        this->m_UserProfile = new es::user::UserProfile(this);
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

    user::UserProfile *PluginManager::GetUserProfile(void)
    {
        return m_UserProfile;
    }

    transcript::Transcriptor *PluginManager::GetTranscriptor(void)
    {
        return m_Transcriptor;
    }

    subtitles::SubtitlesManager *PluginManager::GetSubtitlesManager(void)
    {
        return m_SubtitlesManager;
    }

    obs::TopAudioMic *PluginManager::GetTopAudioMicManager(void)
    {
        return m_topAudioMicManager;
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
        transcript::Transcriptor *tm = pm->m_Transcriptor;

        // std::this_thread::sleep_for(std::chrono::seconds(10));
        tm->run(pm);
    }

    void PluginManager::RunEchostra(void *private_data)
    {
        PluginManager *pm = static_cast<PluginManager *>(private_data);

#if BUILD_TYPE == RELEASE_EASYSTREAM
#ifdef _WIN32
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        // std::string command("cd ./Echostra/Echostra-main/ && python src.main");
        TCHAR command[] = _T("cmd.exe /K bash -c \"cd ../../obs-plugins/64bit/Echostra/Echostra-main/ && source ./EasystreamEnv/Scripts/Activate && python -m src.main \"");
        // const TCHAR command[48] = (TCHAR *)"cd ./Echostra/Echostra-main/ && python src.main";
        wchar_t wtext[48];

        // mbstowcs(wtext, command, 49);
        ZeroMemory(&(pm->_siEchostra), sizeof((pm->_siEchostra)));
        pm->_siEchostra.cb = sizeof(pm->_siEchostra);
        ZeroMemory(&(pm->_piEchostra), sizeof((pm->_piEchostra)));

        if (!CreateProcess(
            _T("C:\\Windows\\System32\\cmd.exe"),
            command,
            // reinterpret_cast<LPWSTR>("cd ./Echostra/Echostra-main/ && python src.main"),
            NULL,
            NULL,
            FALSE,
            CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT,
            NULL,
            NULL,
            &pm->_siEchostra,
            &pm->_piEchostra
        )) {
            std::cerr << "process not created " << GetLastError() << std::endl;

        } else {
        }

#endif

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
            char command[] = "cd ~/Echostra/Echostra-main/ && source ./easystreamEnv/scripts/Activate && python -m src.main";
            system(command);
        }
#endif
#endif
    }
}