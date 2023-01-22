/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ActionWordDetect
*/

#ifndef ACTIONWORDDETECT_HPP_
#define ACTIONWORDDETECT_HPP_

#include "../Action.hpp"

namespace es
{
    class ActionWordDetect : public Action
    {
    public:
        ActionWordDetect(Reaction *reaction, const size_t &area_id, const json &param);
        ~ActionWordDetect();

        virtual void Solve() override;

    protected:
    private:
        std::string _word = "";
    };
}

#endif /* !ACTIONWORDDETECT_HPP_ */
