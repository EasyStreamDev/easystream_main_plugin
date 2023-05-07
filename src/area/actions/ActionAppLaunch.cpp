/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ActionAppLaunch
*/

#include "ActionAppLaunch.hpp"
#include "../../utils/Window.hpp"

namespace es::area
{
    ActionAppLaunch::ActionAppLaunch(Reaction *reaction, const size_t &area_id, const json &param)
        : Action(reaction, area_id, param)
    {
        std::cerr << "oooooooooooooooooooooooo getting appname" << std::endl;
        _appName = param.at("app_name").get<std::string>();
        std::cerr << "oooooooooooooooooooooooo got appname" << std::endl;
        oldWindowsList = utils::window::GetWindowList();
    }

    ActionAppLaunch::~ActionAppLaunch()
    {
    }

    void ActionAppLaunch::Solve()
    {
        std::vector<std::string> windowsList = utils::window::GetWindowList();

        if (std::find(windowsList.begin(), windowsList.end(), _appName) != windowsList.end() &&
            std::find(oldWindowsList.begin(), oldWindowsList.end(), _appName) == oldWindowsList.end())
        {
            this->_isTrue = true;
        }

        oldWindowsList = utils::window::GetWindowList();
    }

    action_t ActionAppLaunch::ToStruct()
    {
        return {
            _id,
            ActionType::APP_LAUNCH,
            {{"app_name", _appName}}};
    }
}
