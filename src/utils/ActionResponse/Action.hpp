/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** Action
*/

#ifndef ACTION_HPP_
#define ACTION_HPP_

#include "../../Common.hpp"
#include "Response.hpp"

namespace es
{
    class Action
    {
    public:
        Action(Response *response);
        ~Action();

        virtual void Solve() = 0;

        void ActivateResponse() const;

        const bool IsTrue() const;
        void Reset();

    protected:
        bool _isTrue = false;
        Response *_response = nullptr;

    private:
    };
}

#endif /* !ACTION_HPP_ */
