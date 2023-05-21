/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ActionWordDetect
*/

#ifndef ACTIONWORDDETECT_HPP_
#define ACTIONWORDDETECT_HPP_

#include "../Action.hpp"

namespace es::area
{
    class ActionWordDetect : public Action
    {
    public:
        ActionWordDetect(Reaction *reaction, const size_t &area_id, const json &param);
        ~ActionWordDetect();

        virtual void Solve() override;
        virtual es::area::action_t ToStruct() override;

    protected:
    private:
        std::vector<std::string> _words;
    };
}

#endif /* !ACTIONWORDDETECT_HPP_ */
