/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** TestReaction
*/

#ifndef TESTREACTION_HPP_
#define TESTREACTION_HPP_

#include "../Reaction.hpp"

namespace es
{
    class TestReaction : public Reaction
    {
    public:
        TestReaction(const size_t &area_id);
        ~TestReaction();

        virtual void Resolve() override;

    protected:
    private:
    };
}

#endif /* !TESTREACTION_HPP_ */
