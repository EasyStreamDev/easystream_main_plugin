// #ifndef TRANSCRIPTOR_HPP
// #define TRANSCRIPTOR_HPP

// #include <string>
// #include <cpprest/ws_client.h>
// #include <cpprest/streams.h>
// #include <cpprest/filestream.h>

// #include "../../../nlohmann/json.hpp"

// namespace ws_socket = web::websockets::client;

// using WS_CallbackClient = ws_socket::websocket_callback_client;
// using WS_OutMessage = ws_socket::websocket_outgoing_message;
// using WS_InMessage = ws_socket::websocket_incoming_message;
// using json = nlohmann::json;

// namespace es::transcription
// {
//     class Transcriptor
//     {
//         /* USEFUL DEFINITIONS */
//     public:
//         using ResponseCallback = void (*)(const json &);
//         enum Status
//         {
//             CONNECTING,
//             CONNECTED,
//             TRANSCRIPTING,
//             DISCONNECTING,
//             DISCONNECTED,
//         };

//     public:
//         /* CTOR & DTOR */
//         Transcriptor();
//         Transcriptor(const std::string, ResponseCallback = nullptr);
//         ~Transcriptor();

//         /* PUBLIC METHODS */
//         void start();
//         void stop();
//         void sendAudio(const std::string &);

//         /* SETTERS */
//         void setResponseCallback(ResponseCallback);
//         void setAccessToken(const std::string &);
//         inline void setStatus(const Status &s_) { this->status = s_; }

//         /* GETTERS */
//         inline const Status &getStatus(void) { return this->status; }
//         inline const ResponseCallback &getResponseCallback(void) { return this->transcription_callback; }

//         /* OPERATORS */
//     public:
//         Transcriptor &operator=(const Transcriptor &t_);

//         /* PRIVATE METHODS */
//     private:
//         void connect();
//         void disconnect();

//         /* MEMBER VARIABLES */
//     private:
//         WS_CallbackClient client;
//         std::string accessToken;
//         const std::string websocketEndpoint = "wss://api.rev.ai/speechtotext/v1/stream";
//         const std::string contentTypeWAV = "&content_type=audio/x-wav";
//         const std::string _tmp_f = "./tmp/tmp_stream_file";
//         std::string url;
//         ResponseCallback transcription_callback = nullptr;
//         Transcriptor::Status status = Status::DISCONNECTED;
//     };
// }

// #endif // TRANSCRIPTOR_HPP