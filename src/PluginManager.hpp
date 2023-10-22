/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** PluginManager
*/

#ifndef PLUGINMANAGER_HPP_
#define PLUGINMANAGER_HPP_

#define MAX_THREAD_NUMBER 10
#define SERVER_HOST "0.0.0.0"
#define SERVER_PORT 47920

#include "utils/Thread.hpp"
#include "area/AreaManager.hpp"
#include "server/include/AsioTcpServer.hpp"
#include "obs/SourceTracker.hpp"
#include "obs/sceneSwitcherAI/SceneSwitcherAI.hpp"
#include "obs/subtitles/SubtitlesManager.hpp"
#include "obs/speechRecognition/Transcriptor.hpp"
#include "obs/profile/UserProfile.hpp"

#include "IPluginManager.hpp"

#ifdef unix
#include <string.h>
#include <signal.h>
#include <unistd.h>
#endif
namespace es
{
    class PluginManager : public IPluginManager
    {
    public:
        PluginManager();
        ~PluginManager();

        void Init(void);
        void Start(void);
        void Stop(void);
        const std::atomic<bool> &IsRunning(void) const;

    public:
        area::AreaManager *GetAreaMain(void) final;
        server::IServer *GetServer(void) final;
        obs::SourceTracker *GetSourceTracker(void) final;
        thread::ThreadPool *GetThreadPool(void) final;
        transcript::Transcriptor *GetTranscriptor(void) final;
        subtitles::SubtitlesManager *GetSubtitlesManager(void) final;
        user::UserProfile *GetUserProfile(void) final;

    private:
        // Asynchrounous routines (run in separate threads)
        static void RunServer(void *);
        static void RunArea(void *);
        static void RunSceneSwitcherAI(void *);
        static void RunSubTitles(void *);
        static void RunTranscriptor(void *);
        static void RunRecorder(void *);
        static void RunEchostra(void *);

    private:
#ifdef unix
        pid_t _pyProgramPid;
#endif
        std::atomic<bool> m_Running = false;
        user::UserProfile *m_UserProfile;

        obs::SourceTracker *m_SourceTracker = nullptr; // @dev : should auto-leveler be separate runnable ?
        thread::ThreadPool *m_ThreadPool = nullptr;

        // Runnable
        std::atomic<area::AreaManager *> m_AreaManager = nullptr;
        std::atomic<server::AsioTcpServer *> m_Server = nullptr;
        std::atomic<subtitles::SubtitlesManager *> m_SubtitlesManager = nullptr;
        transcript::Transcriptor *m_Transcriptor = nullptr;
        std::unordered_map<std::string, obs::SourceRecorder *> _recorders;
    };
} // namespace es

#endif /* !PLUGINMANAGER_HPP_ */