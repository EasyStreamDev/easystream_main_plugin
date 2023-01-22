/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ActionAppLaunch
*/

#include "ActionAppLaunch.hpp"
#include "../../Window.hpp"

es::ActionAppLaunch::ActionAppLaunch(Reaction *reaction, const size_t &area_id, const json &param)
    : Action(reaction, area_id, param)
{
    _appName = param.at("app_name").get<std::string>();
    oldWindowsList = es::utils::window::GetWindowList();
}

es::ActionAppLaunch::~ActionAppLaunch()
{
}

void es::ActionAppLaunch::Solve()
{
    std::vector<std::string> windowsList = es::utils::window::GetWindowList();

    if (std::find(windowsList.begin(), windowsList.end(), _appName) != windowsList.end() &&
        std::find(oldWindowsList.begin(), oldWindowsList.end(), _appName) == oldWindowsList.end())
    {
        this->_isTrue = true;
    }

    oldWindowsList = es::utils::window::GetWindowList();
}

es::area::action_t es::ActionAppLaunch::ToStruct()
{
    return {
        _id,
        es::area::ActionType::APP_LAUNCH,
        {{"app_name", _appName}}
    };
}