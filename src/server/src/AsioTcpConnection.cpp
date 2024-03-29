/*
** EPITECH PROJECT, 2021
** NetworkCommunication
** File description:
** AsioTcpConnection
*/

#include "../include/AsioTcpConnection.hpp"

namespace es::server
{
    AsioTcpConnection::AsioTcpConnection(asio::ip::tcp::socket &sock)
        : _connected(true), _socket(std::move(sock))
    {
    }

    AsioTcpConnection::~AsioTcpConnection()
    {
    }

    void AsioTcpConnection::readMessage()
    {
        if (!_socket.is_open())
        {
            std::cout << "Socket is closed" << std::endl;
            return;
        }

        std::memset(_receiver, 0, MSGMAX);
        _socket.async_read_some(
            asio::buffer(_receiver, MSGMAX),
            [this](asio::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    std::string msg(_receiver);
                    try
                    {
                        std::cout << "[SERVER EASYSTREAM]: trying to reand ((" << msg << "))" << std::endl;
                        json tmp = json::parse(msg);
                        _messages.push_back(tmp);
                        std::cout << "[SERVER EASYSTREAM]: message received: " << tmp << std::endl;
                    }
                    catch (const nlohmann::detail::parse_error &e)
                    {
                        std::cout << "[SERVER EASYSTREAM]: bad message: " << e.what() << std::endl;
                        std::cout << "[SERVER EASYSTREAM]: closing socket" << std::endl;
                        _socket.close();
                        _connected = false;
                        return;
                    }
                }
                else if (ec == asio::error::eof)
                {
                    std::cout << "[SERVER EASYSTREAM]: Socket has been disconnected" << std::endl;
                    _connected = false;
                    _socket.close();
                }
                else
                {
                    std::cout << "[SERVER EASYSTREAM]: ERROR READING MESSAGE: " << ec.message() << std::endl;
                    // _connected = false;
                    return;
                }
                readMessage();
            });
    }

    void AsioTcpConnection::writeMessage(const std::string &msg)
    {
        std::scoped_lock tmp(this->_writeMutex);
        // char buffer[MSGMAX];
        const std::string f_msg = msg + "\r\n";

        std::cout << "msg: " << f_msg << std::endl;
        std::memset(_buffer, 0, MSGMAX);
        std::memcpy(_buffer, f_msg.data(), f_msg.size());
        asio::async_write(
            _socket,
            asio::buffer(_buffer, f_msg.size()),
            [this](asio::error_code ec, std::size_t length)
            {
                (void)length;
                if (!ec)
                {
                    std::cout << "[SERVER EASYSTREAM]: Message sent to " << _socket.remote_endpoint() << std::endl;
                }
                else
                {
                    std::cout << "[SERVER EASYSTREAM]: Write failed: " << ec.message() << std::endl;
                    _connected = false;
                    _socket.close();
                }
            });
    }

    const asio::ip::tcp::socket &AsioTcpConnection::getSocket() const
    {
        return (_socket);
    }

    const std::vector<nlohmann::json> AsioTcpConnection::getMessage()
    {
        std::vector<nlohmann::json> tmp = _messages;

        _messages.clear();
        return (tmp);
    }

    bool AsioTcpConnection::isConnected() const
    {
        return (_connected);
    }
}
