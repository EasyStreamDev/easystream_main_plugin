#include "DataStream.hpp"

es::transcript::DataStream::DataStream() : _port(0)
{
    std::cerr << "opened DataStream" << std::endl;
}

es::transcript::DataStream::~DataStream()
{
#ifdef unix
    close(_sock);
#elif _WIN32
    if (!_closed)
    {
        std::cerr << "closed DataStream" << std::endl;
        closesocket(_sock);
        WSACleanup();
    }
#endif
    std::cerr << "closed DataStream" << std::endl;
}

int es::transcript::DataStream::getPort() const
{
    std::cerr << "port is " << _port << std::endl;
    return _port;
}

void es::transcript::DataStream::setPort(int port)
{
    std::cerr << "setting port" << std::endl;
    _port = port;
}

bool es::transcript::DataStream::connectToTranscription()
{
    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(_port);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

#ifdef unix
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_sock == -1)
    {
        std::cerr << "[EASYTREAM]: Could not create socket" << std::endl;
        return false;
    }

    if (connect(_sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
    {
        std::cerr << "[EASYTREAM]: could not connect to other socket" << std::endl;
        return false;
    }
#elif _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0)
    {
        std::cerr << "[EASYSTREAM] WSAStartup failed" << std::endl;
        return false;
    }

    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_sock == INVALID_SOCKET)
    {
        std::cerr << "[EASYTREAM]: Could not create socket" << std::endl;
        WSACleanup();
        return false;
    }

    if (connect(_sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
    {
        std::cerr << "[EASYTREAM]: could not connect to other socket" << std::endl;
        WSACleanup();
        return false;
    }
    std::cerr << "Connected socket" << std::endl;

    _closed = false;
#endif
    return true;
}

bool es::transcript::DataStream::sendMessage(std::string message)
{
    int toSendSize = 0;

    while (message.size() != 0)
    {
        int size = message.size() + _queue.size();

        if (size < 1024)
        {
            _queue += message;
            break;
        }
        else
        {
            std::string toSend = message.substr(0, 1024 - _queue.size());
            toSendSize = toSend.size();
            _queue.append(toSend);
            message = message.substr(toSendSize);
#ifdef unix
            size_t sentSize = send(_sock, _queue.c_str(), _queue.size(), 0);
            if (sentSize == -1)
            {
                return false;
            }
            _queue.clear();
#elif _WIN32
            int value = send(_sock, _queue.c_str(), _queue.size(), 0);
            if (value == SOCKET_ERROR)
            {
                std::cerr << "[EASYTREAM]: could not send message to other socket" << std::endl;
                return false;
            }
            else
            {
                std::cerr << "Message sent to socket python " << strlen(message) << " bytes against " << _queue.length() << " against " << _queue.size() << std::endl;
                message = message.substr(toSendSize);
                // int oS = _queue.size();
                _queue = "";
                // std::cerr << _queue.size() <<
                // sendMessage(message.substr(1024 - oS).c_str());
            }

#endif
        }
    }
    return true;
}