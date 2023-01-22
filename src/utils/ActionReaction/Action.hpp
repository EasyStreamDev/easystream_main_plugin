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
        Action(Reaction *reaction, const size_t &area_id, const json &param);
        ~Action();

        virtual void Solve() = 0;

        void ActivateResponse() const;

        const bool IsTrue() const;
        void Reset();

        inline void SetId(const size_t &id) { this->_id = id; }
        void SetReaction(Reaction *);

        inline const size_t &GetId() const { return this->_id; }
        inline Reaction *GetReaction() const { return this->_reaction; }

        virtual es::area::action_t ToStruct() = 0;

    protected:
        bool _isTrue = false;
        Reaction *_reaction = nullptr;
        size_t _id;
    };
}

#endif /* !ACTION_HPP_ */
