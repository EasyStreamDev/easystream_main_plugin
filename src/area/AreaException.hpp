/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** AreaException
*/

#include <string>

namespace es::area
{
    class AreaException : public std::exception
    {
    public:
        explicit AreaException(const std::string &message) : message_(message) {}

        const char *what() const noexcept override
        {
            return message_.c_str();
        }

    private:
        std::string message_;
    };
}