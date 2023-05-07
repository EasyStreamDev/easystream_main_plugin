/*
** EPITECH PROJECT, 2021
** NetworkCommunication
** File description:
** IServer
*/

#ifndef ISERVER_HPP_
#define ISERVER_HPP_

// #include "Error.hpp"

// Linked
#include <asio.hpp>
#include <iostream>

namespace es::server
{
    class IServer
    {
    public:
        virtual bool start() = 0;
        virtual void submitBroadcast(const json &) = 0;
        virtual asio::io_context &getContext() = 0;
    };
}

#endif /* !ISERVER_HPP_ */
