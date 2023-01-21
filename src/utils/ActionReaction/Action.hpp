/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** Action
*/

#ifndef ACTION_HPP_
#define ACTION_HPP_

#include "../../Common.hpp"
#include "Reaction.hpp"

namespace es
{
    class Action
    {
    public:
        Action(Reaction *reaction, const size_t &area_id);
        ~Action();

        virtual void Solve() = 0;

        void ActivateResponse() const;

        const bool IsTrue() const;
        void Reset();

        inline const size_t &GetId() const { return this->_id; }

    protected:
        bool _isTrue = false;
        Reaction *_reaction = nullptr;
        const size_t _id;
    };
}

#endif /* !ACTION_HPP_ */
