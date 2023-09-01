#include "DataStream.hpp"

es::transcript::DataStream::DataStream() : _port(0), _closed(true)
{
}

es::transcript::DataStream::~DataStream()
{
    if (!_closed)
    {
#ifdef unix
        close(_sock);
#elif _WIN32
        closesocket(_sock);
        WSACleanup();
#endif
}
}

int es::transcript::DataStream::getPort() const
{
    return _port;
}

void es::transcript::DataStream::setPort(int port)
{
    _port = port;
}

bool es::transcript::DataStream::connectToTranscription()
{
    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(_port);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

#ifdef unix
    {
        std::unique_lock lock(_mtx);
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
        _closed = false;
    }
#elif _WIN32
    {
        std::unique_lock lock(_mtx);
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
        _closed = false;
    }
#endif
    std::cerr << "connected to socket python" << std::endl;
    return true;
}

void es::transcript::DataStream::disconnectSocket()
{
    #ifdef unix
        close(_sock);
    #elif _WIN32
        closesocket(_sock);
        // WSACleanup();
    #endif
    _closed = true;
}

bool es::transcript::DataStream::sendMessage(std::string message)
{
    int toSendSize = 0;

    while (message.size() != 0 && !_closed)
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
            std::size_t sentSize = 0;
            {
                std::unique_lock lock(_mtx);
                sentSize = send(_sock, _queue.c_str(), _queue.size(), 0);
            }
            if (sentSize == -1)
            {
                return false;
            }
            _queue.clear();
#elif _WIN32
            int value = 0;
            {
                std::unique_lock lock(_mtx);
                value = send(_sock, _queue.c_str(), _queue.size(), 0);
            }
            // std::cerr << "sent " << value  << "to python" << std::endl;
            if (value == SOCKET_ERROR)
            {
                return false;
            }
            else
            {
                _queue.clear();
            }

#endif
        }
    }
    return true;
}