/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ActionResponseMain
*/

#include "ActionResponseMain.hpp"

namespace es
{
    ActionResponseMain::ActionResponseMain()
    {
    }

    ActionResponseMain::~ActionResponseMain()
    {
        this->_actions_mutex.lock();
        for (Action *action : this->_actions)
        {
            delete action;
        }
        this->_actions.clear();
        this->_actions_mutex.unlock();
    }

    void ActionResponseMain::run()
    {
        while (1)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            this->Update();
        }
    }

    void ActionResponseMain::Update()
    {
        this->_actions_mutex.lock();
        for (Action *action : _actions)
        {
            action->Solve();

            if (action->IsTrue())
            {
                action->ActivateResponse();
            }

            action->Reset();
        }
        this->_actions_mutex.unlock();
    }

    void ActionResponseMain::AddAction(Action *action)
    {
        this->_actions_mutex.lock();
        _actions.push_back(action);
        this->_actions_mutex.unlock();
    }
}
