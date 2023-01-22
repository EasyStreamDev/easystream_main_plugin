/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ActionAppLaunch
*/

#ifndef ACTIONAPPLAUNCH_HPP_
#define ACTIONAPPLAUNCH_HPP_

#include "../Action.hpp"

namespace es
{
    class ActionAppLaunch : public Action
    {
    public:
        ActionAppLaunch(Reaction *reaction, const size_t &area_id, const json &param);
        ~ActionAppLaunch();

        virtual void Solve() override;

    protected:
    private:
        std::string _appName = "";
        std::vector<std::string> oldWindowsList;

    };
}

#endif /* !ACTIONAPPLAUNCH_HPP_ */
