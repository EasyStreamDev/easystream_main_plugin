#ifndef TRANSCRIPTOR_HPP
#define TRANSCRIPTOR_HPP

#include "../../../Common.hpp"
#include "./ITranscriptorManager.hpp"

#include <cpprest/ws_client.h>
#include <cpprest/streams.h>
#include <cpprest/filestream.h>

namespace ws_socket = web::websockets::client;

using WS_CallbackClient = ws_socket::websocket_callback_client;
using WS_OutMessage = ws_socket::websocket_outgoing_message;
using WS_InMessage = ws_socket::websocket_incoming_message;
using TimePoint = std::chrono::_V2::steady_clock::time_point;

namespace es::transcription
{
    using String = std::string;

    template <typename T>
    using Optional = std::optional<T>;

    template <typename T>
    using Queue = std::queue<T>;

    template <typename T>
    using Vector = std::vector<T>;

    template <typename T1, typename T2>
    using Map = std::map<T1, T2>;

    template <typename T1, typename T2>
    using Umap = std::unordered_map<T1, T2>;

    template <typename T1, typename T2>
    using Pair = std::pair<T1, T2>;

    class Transcriptor
    {
        /* USEFUL DEFINITIONS */
    public:
        // using ResponseCallback = void (*)(const json &);
        enum Status
        {
            CONNECTING,
            CONNECTED,
            TRANSCRIPTING,
            DISCONNECTING,
            DISCONNECTED,
        };

    public:
        /* CTOR & DTOR */
        Transcriptor() = default;
        Transcriptor(ITranscriptorManager *);
        ~Transcriptor();

        /* PUBLIC METHODS */
        void init(const std::string &);
        void start();
        void stop();
        void sendAudio(const uint &, const std::string &);

        /* SETTERS */
        void setAccessToken(const std::string &);
        inline void setStatus(const Status &s_) { this->status = s_; }

        /* GETTERS */
        inline const Status &getStatus(void) { return this->status; }
        inline ITranscriptorManager *getManager(void) { return m_TSManager; }
        const int64_t getMsElapsedSinceConnection(void) const;

        /* OPERATORS */
    public:
        Transcriptor &operator=(const Transcriptor &t_);

        /* PRIVATE METHODS */
    private:
        void connect();
        void disconnect();

        /* MEMBER VARIABLES */
    private:
        // Pointer to manager (for transcription saving)
        ITranscriptorManager *m_TSManager = nullptr;

        // Current transcripting file data
        ts_result_t m_FileData = {};

        // WebSocket
        WS_CallbackClient client;
        std::string accessToken;
        Transcriptor::Status status = Status::DISCONNECTED;
        const std::string websocketEndpoint = "wss://api.rev.ai/speechtotext/v1/stream";
        const std::string contentTypeWAV = "&content_type=audio/x-wav";
        const std::string _tmp_f = "./tmp/tmp_stream_file";
        std::string url;

        // Timeout stopwatch
        TimePoint _request_start = {};
    };
}

#endif // TRANSCRIPTOR_HPP