#ifndef COMMUNICATION_ERRORS_HPP
#define COMMUNICATION_ERRORS_HPP
#include <iostream>

#define NOT_FOUND_SERV 404
#define BAD_ARGS_SERV 400
#define ERROR_WRONG_INFORMATION 500
#define STATUS_OK 200

namespace es::server
{
    class RequestError : public std::exception
    {
    public:
        RequestError(const char *message) : errorMessage(message) {}

        const char *what() const noexcept override
        {
            return errorMessage.c_str();
        }

    private:
        const std::string errorMessage;
    };
}

#endif