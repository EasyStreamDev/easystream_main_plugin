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
    std::map<std::string, int> KeyStrToKeyCode{
        {"0", 0x30},
        {"1", 0x31},
        {"2", 0x32},
        {"3", 0x33},
        {"4", 0x34},
        {"5", 0x35},
        {"6", 0x36},
        {"7", 0x37},
        {"8", 0x38},
        {"9", 0x39},
        {"a", 0x41},
        {"b", 0x42},
        {"c", 0x43},
        {"d", 0x44},
        {"e", 0x45},
        {"f", 0x46},
        {"g", 0x47},
        {"h", 0x48},
        {"i", 0x49},
        {"j", 0x4A},
        {"k", 0x4B},
        {"l", 0x4C},
        {"m", 0x4D},
        {"n", 0x4E},
        {"o", 0x4F},
        {"p", 0x50},
        {"q", 0x51},
        {"r", 0x52},
        {"s", 0x53},
        {"t", 0x54},
        {"u", 0x55},
        {"v", 0x56},
        {"w", 0x57},
        {"x", 0x58},
        {"y", 0x59},
        {"z", 0x5A},
    };

    ActionKeyPressed::ActionKeyPressed(Reaction *reaction, const size_t &area_id, const json &param)
        : Action(reaction, area_id, param)
    {
        _key = param["key"].get<std::string>();
        std::transform(_key.begin(), _key.end(), _key.begin(),
                       [](unsigned char c)
                       { return std::tolower(c); });

        _CTRL_modifier = true;  // param["ctrl_modifier"].get<bool>();
        _ALT_modifier = true;   // param["alt_modifier"].get<bool>();
        _SHIFT_modifier = false; // param["shift_modifier"].get<bool>();
    }

    ActionKeyPressed::~ActionKeyPressed()
    {
    }

    void ActionKeyPressed::Solve()
    {
        if (IsKeyPressed(_key))
        {
            this->_isTrue = true;
        }
    }

    action_t ActionKeyPressed::ToStruct()
    {
        return {
            _id,
            ActionType::KEY_PRESSED,
            {{"key", _key},
             {"ctrl_modifier", _CTRL_modifier},
             {"alt_modifier", _ALT_modifier},
             {"shift_modifier", _SHIFT_modifier}}};
    }

    bool ActionKeyPressed::IsKeyPressed(std::string KeyStr)
    {
#ifdef _WIN32 // Windows
        // Windows implementation
        if (_CTRL_modifier && !(GetAsyncKeyState(VK_CONTROL) & 0x8000))
            return false;
        if (_ALT_modifier && !(GetAsyncKeyState(VK_MENU) & 0x8000))
            return false;
        if (_SHIFT_modifier && !(GetAsyncKeyState(VK_SHIFT) & 0x8000))
            return false;

        return (GetAsyncKeyState(KeyStrToKeyCode[KeyStr]) & 0x8000) != 0;
#elif __linux__ // Linux
        // Linux implementation
        Display *display = XOpenDisplay(nullptr);
        KeySym sym_a = XStringToKeysym(KeyStr.c_str());
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