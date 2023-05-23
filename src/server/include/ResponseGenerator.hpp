/*
** NetworkCommunication
** File description:
** ResponseGenerator
*/

#ifndef RESPONSEGENERATOR_HPP_
#define RESPONSEGENERATOR_HPP_

#include "../../utils/Json.hpp"

namespace es::server
{
    class ResponseGenerator
    {
    public:
        ResponseGenerator() = delete;
        ResponseGenerator(const ResponseGenerator &) = delete;
        ResponseGenerator(ResponseGenerator &&) = delete;

    public:
        static const json Success(const std::string & = "OK", const json & = {});
        static const json BadCommand(const std::string & = "The requested action does not exist.");
        static const json BadRequest(const std::string & = "");
        static const json NotFound(const std::string & = "");
    };
}

#endif /* RESPONSEGENERATOR_HPP_ */