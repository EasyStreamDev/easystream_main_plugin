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
// #include "obs/speechRecognition/record/SourceRecorder.hpp"
#include "PluginManager.hpp"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

es::PluginManager *g_PluginManager = nullptr;
os_cpu_usage_info_t *g_CpuUsageInfo = nullptr;

namespace _comment
{
    // const int SERV_PORT = 47920;
    // es::area::AreaManager g_ARmain;

    // void startSpeechRecognition(void *)
    // {
    //     thread_sleep_ms(2000);
    //     blog(LOG_INFO, "###  - Speech recognition starting...");

    //     // es::TranscriptorManager tm;
    //     obs_source_t *source = obs_get_source_by_name("Mic/Aux");
    //     blog(LOG_INFO, "###  - Speech recognition started.");

    //     if (source)
    //     {
    //         es::obs::SourceRecorder recorder(source);

    //         // tm.start();
    //         while (1)
    //         {
    //             // Feed files to the transcriptor manager using \
//             // the tm.transcriptFile function (callback parameter)
    //             // thread_sleep_ms(5);
    //         }
    //     }
    //     // tm.stop();
    //     blog(LOG_INFO, "###  - Speech recognition has ended.");
    // }

    // bool obs_module_load(void)
    // {
    //     blog(LOG_INFO, "###  - Plugin loaded successfully (version %s)", PLUGIN_VERSION);
    //     blog(LOG_INFO, "### -----------------------------------------");

    //     tracker->init();

    //     threadPool->push(std::function(startAREASystem), nullptr);
    //     threadPool->push(std::function(startServer), nullptr);
    //     threadPool->push(std::function(startSpeechRecognition), nullptr);
    //     threadPool->push(std::function(sceneSwitcherIA), nullptr);

    //     g_CpuUsageInfo = os_cpu_usage_info_start();

    //     blog(LOG_INFO, "### -----------------------------------------");

    //     return true;
    // }
}

os_cpu_usage_info_t *GetCpuUsageInfo()
{
    return g_CpuUsageInfo;
}

bool obs_module_load(void)
{
    blog(LOG_INFO, "###  - Plugin loaded successfully (version %s)", PLUGIN_VERSION);
    blog(LOG_INFO, "### -----------------------------------------");

    g_PluginManager = new es::PluginManager;
    g_PluginManager->Init();
    g_PluginManager->Start();

    g_CpuUsageInfo = os_cpu_usage_info_start();

    blog(LOG_INFO, "### -----------------------------------------");
    return true;
}

void obs_module_unload()
{
    g_PluginManager->Stop();

    os_cpu_usage_info_destroy(g_CpuUsageInfo);

    blog(LOG_INFO, "### Plugin unloaded");
}