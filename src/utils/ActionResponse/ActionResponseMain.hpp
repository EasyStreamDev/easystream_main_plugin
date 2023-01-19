/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ActionResponseMain
*/

#ifndef ACTIONRESPONSEMAIN_HPP_
#define ACTIONRESPONSEMAIN_HPP_

#include "../../Common.hpp"
#include "Action.hpp"

namespace es
{
    class ActionResponseMain
    {
    public:
        ActionResponseMain();
        ~ActionResponseMain();

        void run();

        void Update();
        void AddAction(Action *action);

    private:
        std::mutex _actions_mutex;
        std::vector<Action *> _actions;
    };
}

#endif /* !ACTIONRESPONSEMAIN_HPP_ */
