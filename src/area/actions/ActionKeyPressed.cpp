/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ActionKeyPressed
*/

#include "ActionKeyPressed.hpp"


#ifdef _WIN32 // Windows
    #include <Windows.h>
#elif __linux__ // Linux
    #include <X11/Xlib.h>
#endif

namespace es::area
{
    ActionKeyPressed::ActionKeyPressed(Reaction *reaction, const size_t &area_id, const json &param)
        : Action(reaction, area_id, param)
    {
        _key = param["key"].get<std::string>();
    }

    ActionKeyPressed::~ActionKeyPressed()
    {
    }

    void ActionKeyPressed::Solve()
    {
        if (IsKeyPressed(0))
        {
            this->_isTrue = true;
        }
    }

    action_t ActionKeyPressed::ToStruct()
    {
        return {
            _id,
            ActionType::KEY_PRESSED,
            {{"key", _key}}};
    }

    bool ActionKeyPressed::IsKeyPressed(int keyCode) {
        #ifdef _WIN32 // Windows
            // Windows implementation
            return (GetAsyncKeyState(keyCode) & 0x8000) != 0;
        #elif __linux__ // Linux
            // Linux implementation
            Display* display = XOpenDisplay(nullptr);
            KeySym sym_a = XStringToKeysym(_key.c_str());
            KeyCode xKeyCode = XKeysymToKeycode(display, sym_a);
            char keys[32];

            XQueryKeymap(display, keys);

            bool isKeyPressed = (keys[xKeyCode / 8] & (1 << (xKeyCode % 8))) != 0;

            XCloseDisplay(display);

            return isKeyPressed;
        #else
            // Default implementation for other platforms
            // Handle unsupported platform or key code
            // Return an appropriate default value or throw an exception
            return false;
        #endif
    }
}
