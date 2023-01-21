/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** TestAction
*/

#ifndef TESTACTION_HPP_
#define TESTACTION_HPP_

#include "../Action.hpp"

namespace es
{
    class TestAction : public Action
    {
    public:
        TestAction(Reaction *reaction, const size_t &area_id);
        ~TestAction();

        virtual void Solve() override;

    protected:
    private:
    };
}

#endif /* !TESTACTION_HPP_ */
