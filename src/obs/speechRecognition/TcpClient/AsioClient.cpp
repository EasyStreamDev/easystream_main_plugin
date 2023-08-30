#include "AsioClient.hpp"


es::transcript::AsioClient::AsioClient(const std::atomic_bool &running): _sock(_ctx), _running(running)
{
}

bool es::transcript::AsioClient::connectToServer()
{
    try {
        asio::ip::tcp::resolver resolver(_ctx);
        asio::ip::tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "47921");

        asio::connect(_sock, endpoints);
        std::cerr << "AsioClient Okay" << std::endl;
        return true;
    } catch (const asio::system_error &e) {
        std::cerr << "[EASYSTREAM TCP CLIENT]: could not connect to echostra" << std::endl;
        // std::cerr << "AsioClient Okay" << std::endl;
        return false;
    }
}

void es::transcript::AsioClient::readMessage()
{
    std::cerr << "readMessage before" << std::endl;
    if (!_running)
        return;

    std::cerr << "readMessage" << std::endl;
    std::memset(_read, 0, MSGMAX);
    asio::error_code ec;

    _sock.read_some(asio::buffer(_read, MSGMAX), ec);
    if (!ec) {
        try {
            std::cerr << "[EASYSTREAM TCP CLIENT]: received message: " << _read << std::endl;
            _push(json::parse(_read));
            // treatMessage(json::parse(_read));
            readMessage();
        } catch (const nlohmann::detail::parse_error &e) {
            std::cerr << "[EASYSTREAM TCP CLIENT]: Formating problem: " << e.what() << std::endl;
            return readMessage();
        }
    } else if (ec == asio::error::eof) {
        std::cerr << "[EASYSTREAM TCP CLIENT]: Server disconnected or has ended connection" << std::endl;
        return readMessage();
    }
    std::cerr << "[EASYSTREAM TCP CLIENT]: error reading message: " << ec.message() << std::endl;
    return readMessage();
}

bool es::transcript::AsioClient::writeMessage(const std::string &msg)
{
    std::memset(_write, 0, MSGMAX);
    std::memcpy(_write, msg.data(), msg.size());
    asio::error_code ec;

    _sock.write_some(asio::buffer(_write, msg.size()), ec);
    if (!ec) {
        std::cerr << "[EASYSTREAM TCP CLIENT]: Message has been succesfully sent to Server" << std::endl;
        return true;
    }
    std::cerr << "[EASYSTREAM TCP SERVER]: failed sending message: " << ec.message() << std::endl;
    return false;
}

// void

void es::transcript::AsioClient::setPushFunc(std::function<void (const json &)> pF)
{
    _push = pF;
}
