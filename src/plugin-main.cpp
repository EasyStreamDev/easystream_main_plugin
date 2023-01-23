/*
obs-text-slideshow
Copyright (C) 2021 Joshua Wong jbwong05@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include "plugin-main.hpp"
#include "src/Server/include/AsioTcpServer.hpp"
#include "obs/speechRecognition/record/SourceRecorder.hpp"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

std::shared_ptr<es::obs::SourceTracker> tracker = std::make_shared<es::obs::SourceTracker>();
std::shared_ptr<es::thread::ThreadPool> threadPool = std::make_shared<es::thread::ThreadPool>(10);

const int SERV_PORT = 47920;
es::ActionReactionMain g_ARmain;

os_cpu_usage_info_t *cpuUsageInfo;

void thread_sleep_ms(uint ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void startSpeechRecognition(std::shared_ptr<void>)
{
    thread_sleep_ms(2000);
    blog(LOG_INFO, "###  - Speech recognition starting...");

    // es::TranscriptorManager tm;
    obs_source_t *source = obs_get_source_by_name("Mic/Aux");
    blog(LOG_INFO, "###  - Speech recognition started.");

    if (source)
    {
        es::obs::SourceRecorder recorder(source);

        // tm.start();
        while (1)
        {
            // Feed files to the transcriptor manager using \
            // the tm.transcriptFile function (callback parameter)
            // thread_sleep_ms(5);
        }
    }
    // tm.stop();
    blog(LOG_INFO, "###  - Speech recognition has ended.");
}

void startAREASystem(std::shared_ptr<void>)
{
    thread_sleep_ms(2000);

    blog(LOG_INFO, "###  - AREA system started.");
    es::area::action_t act1 = {
        0,
        es::area::ActionType::WORD_DETECT,
        {{"words", "juice"}}
    };
    es::area::reaction_t react1 = {
        0,
        "",
        es::area::ReactionType::SCENE_SWITCH,
        {{"scene", "Klondike"}}
    };
    g_ARmain.CreateArea(act1, react1);

    g_ARmain.run(); // Run AREA system loop
    blog(LOG_INFO, "###  - AREA system has stopped.");
}

void startServer(std::shared_ptr<void>)
{
    // thread_sleep_ms(2000);

    blog(LOG_INFO, "###  - Creating server...");
    std::shared_ptr<es::server::AsioTcpServer> server(
        std::make_shared<es::server::AsioTcpServer>(
            std::string("0.0.0.0"),
            SERV_PORT,
            tracker->getAudioMap(),
            &g_ARmain));
    blog(LOG_INFO, "###  - Server created.");

    blog(LOG_INFO, "###  - Starting server...");
    server->start();
    std::chrono::duration<float> mSeconds;
    std::chrono::steady_clock::time_point checkPoint = std::chrono::steady_clock::now();
    blog(LOG_INFO, "###  - Server started. Now running !");
    while (1)
    {
        mSeconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - checkPoint);
        if (mSeconds.count() > 1) { 
            server->update();
            checkPoint = std::chrono::steady_clock::now();
        }
    };
}

void sceneSwitcherIA(std::shared_ptr<void>)
{
    thread_sleep_ms(2000);
    while (1)
    {
        std::vector<std::string> windowsList = es::utils::window::GetWindowList();
        std::vector<json> scenesList = es::utils::obs::listHelper::GetSceneList();
        bool switched = false;

        for (auto &scene : scenesList)
        {
            for (auto &window : windowsList)
            {
                if (scene["sceneName"] == window && !switched)
                {
                    switched = true;
                    if (obs_source_get_name(obs_frontend_get_current_scene()) == scene["sceneName"])
                    {
                        continue;
                    }
                    obs_frontend_set_current_scene(obs_scene_get_source(obs_get_scene_by_name(scene["sceneName"].get<std::string>().c_str())));
                }
            }
        }
    }
}

bool obs_module_load(void)
{
    blog(LOG_INFO, "###  - Plugin loaded successfully (version %s)", PLUGIN_VERSION);
    blog(LOG_INFO, "### -----------------------------------------");

    tracker->init();

    threadPool->push(std::function(startAREASystem), nullptr);
    threadPool->push(std::function(startServer), nullptr);
    threadPool->push(std::function(startSpeechRecognition), nullptr);
    threadPool->push(std::function(sceneSwitcherIA), nullptr);

    cpuUsageInfo = os_cpu_usage_info_start();

    blog(LOG_INFO, "### -----------------------------------------");

    return true;
}

void obs_module_unload()
{
    tracker.reset();
    threadPool.reset();
    os_cpu_usage_info_destroy(cpuUsageInfo);

    blog(LOG_INFO, "### plugin unloaded");
}

os_cpu_usage_info_t *GetCpuUsageInfo()
{
    return cpuUsageInfo;
}