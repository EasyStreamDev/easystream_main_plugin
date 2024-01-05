/*
 EPITECH PROJECT, 2022
 easystream_main_plugin
 File description:
 ActionKeyPressed
*/

#ifndef ACTIONKEYPRESSEDHPP
#define ACTIONKEYPRESSEDHPP

#include "../Action.hpp"

namespace es::area
{
    class ActionKeyPressed : public Action
    {
    public:
        ActionKeyPressed(Reaction *reaction, const size_t &area_id, const json &param);
        ~ActionKeyPressed();

        virtual void Solve() override;
        virtual es::area::action_t ToStruct() override;

        bool IsKeyPressed(std::string KeyStr);

    protected:
    private:
        std::string _key = "";
    
    public:
        bool _ALT_modifier = false;
        bool _CTRL_modifier = false;
        bool _SHIFT_modifier = false;
    };
}

#endif /* !ACTIONKEYPRESSEDHPP */