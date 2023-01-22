/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ActionKeyPressed
*/

#ifndef ACTIONKEYPRESSED_HPP_
#define ACTIONKEYPRESSED_HPP_

#include "../Action.hpp"

namespace es
{
    class ActionKeyPressed : public Action
    {
    public:
        ActionKeyPressed(Reaction *reaction, const size_t &area_id, const json &param);
        ~ActionKeyPressed();

        virtual void Solve() override;

    protected:
    private:
        std::string _key = "";
    };
}

#endif /* !ACTIONKEYPRESSED_HPP_ */
