/*
** NetworkCommunication
** File description:
** ResponseGenerator
*/

#include "../include/ResponseGenerator.hpp"

namespace es::server
{
    const json ResponseGenerator::Success(const std::string &msg, const json &data)
    {
        json toSend;

        toSend["statusCode"] = 200;
        toSend["message"] = msg;
        toSend["data"] = data;

        return toSend;
    }

    const json ResponseGenerator::BadCommand(const std::string &msg)
    {
        json toSend;

        toSend["statusCode"] = 404;
        toSend["message"] = msg;

        return toSend;
    }

    const json ResponseGenerator::BadRequest(const std::string &msg)
    {
        json toSend;

        toSend["statusCode"] = 400;
        toSend["message"] = std::string("Bad request: ") + msg;

        return toSend;
    }

    const json ResponseGenerator::NotFound(const std::string &msg)
    {
        json toSend;

        toSend["statusCode"] = 404;
        if (!msg.empty())
        {
            toSend["message"] = std::string("Not found: ") + msg;
        }
        else
        {
            toSend["message"] = std::string("Not found.");
        }

        return toSend;
    }
}