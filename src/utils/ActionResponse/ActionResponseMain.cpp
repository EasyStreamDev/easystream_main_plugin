/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ActionResponseMain
*/

#include "ActionResponseMain.hpp"

es::ActionResponseMain::ActionResponseMain()
{
}

es::ActionResponseMain::~ActionResponseMain()
{
}

void es::ActionResponseMain::Update()
{
    blog(LOG_INFO, "### [ALGO] Updating");
    for (Action *action : _actions)
    {
        action->Solve();

        if (action->IsTrue())
            action->ActivateResponse();

        action->Reset();
    }
}

void es::ActionResponseMain::AddAction(Action *action)
{
    _actions.push_back(action);
}