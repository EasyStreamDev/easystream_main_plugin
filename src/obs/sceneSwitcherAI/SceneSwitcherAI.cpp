#include "SceneSwitcherAI.hpp"

namespace es::obs::scene_switcher_ai
{
    void run(void *)
    {
        blog(LOG_INFO, "### SCENESWITCHER AI - STARTING...");
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        blog(LOG_INFO, "### SCENESWITCHER AI - STARTED");
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
                        obs_frontend_set_current_scene(
                            obs_scene_get_source(
                                obs_get_scene_by_name(
                                    scene["sceneName"].get<std::string>().c_str())));
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
}