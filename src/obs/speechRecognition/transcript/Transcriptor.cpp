// #include "Transcriptor.hpp"

// namespace es::transcription
// {
//     Transcriptor::Transcriptor()
//     {
//         this->client = WS_CallbackClient();
//         this->setResponseCallback(nullptr);
//         // Set disconnect confirmation callback.
//         this->client.set_close_handler(
//             [&](
//                 ws_socket::websocket_close_status close_status,
//                 const utility::string_t &reason,
//                 const std::error_code &error)
//             {
//                 // this->connected = false;
//                 // this->running = false;
//                 std::cout << "Closing Connection... " << std::flush;
//                 client.close();
//                 this->setStatus(Status::DISCONNECTED);
//                 std::cout << "Connection closed." << std::endl;
//             });
//     }

//     Transcriptor::Transcriptor(const std::string accessToken_, ResponseCallback cb_)
//     {
//         this->client = WS_CallbackClient();
//         this->setAccessToken(accessToken_);
//         // Message received callback
//         this->setResponseCallback(cb_);
//         // Disconnect confirmation callback
//         this->client.set_close_handler(
//             [&](
//                 ws_socket::websocket_close_status close_status,
//                 const utility::string_t &reason,
//                 const std::error_code &error)
//             {
//                 // this->connected = false;
//                 // this->running = false;
//                 std::cout << "Closing Connection... " << std::flush;
//                 client.close();
//                 this->setStatus(Status::DISCONNECTED);
//                 std::cout << "Connection closed." << std::endl;
//             });
//     }

//     Transcriptor::~Transcriptor()
//     {
//         this->stop();
//     }

//     void Transcriptor::start()
//     {
//         // Connect to the WS endpoint.
//         this->connect();
//         std::cout << "Started." << std::endl;
//     }

//     void Transcriptor::stop()
//     {
//         // if (this->connected)
//         if (this->status != Status::DISCONNECTING &&
//             this->status != Status::DISCONNECTED)
//         {
//             this->disconnect();
//         }
//     }

//     void Transcriptor::setAccessToken(const std::string &at_)
//     {
//         this->accessToken = at_;
//         this->url = this->websocketEndpoint + "?access_token=" +
//                     this->accessToken + this->contentTypeWAV;
//     }

//     void Transcriptor::setResponseCallback(ResponseCallback cb_)
//     {
//         this->transcription_callback = cb_;
//         this->client.set_message_handler(
//             [&](ws_socket::websocket_incoming_message msg)
//             {
//                 try
//                 {
//                     json data = json::parse(msg.extract_string().get());
//                     auto response_type = std::string(data["type"]);

//                     if (response_type == "connected")
//                     {
//                         std::cout << "WebSocket Connected" << std::endl;
//                         // this->connected = true;
//                         this->setStatus(Status::CONNECTED);
//                     }
//                     else // if (response_type == "final" || response_type == "partial")
//                     {
//                         ResponseCallback callback = this->getResponseCallback();

//                         if (callback)
//                         {
//                             callback(data);
//                         }
//                         else
//                         {
//                             std::cout << "[Transcriptor] - Response callback undefined." << std::endl;
//                         }
//                     }
//                     // When final response received, send disconnect message.
//                     if (response_type == "final")
//                     {
//                         this->stop();
//                     }
//                 }
//                 catch (std::exception e)
//                 {
//                     std::cerr << e.what() << std::endl;
//                 }
//             });
//     }

//     void Transcriptor::sendAudio(const std::string &file_path)
//     {
//         try
//         {
//             WS_OutMessage msg;
//             concurrency::streams::file_stream<uint8_t> buffer;
//             // buffer.open_istream is asynchronous (returns a task). Using .get() on it
//             // will wait for the task to be fully executed and return the result which here is
//             // an input file stream (type: Concurrency::streams::istream)
//             auto file_input_stream = buffer.open_istream(file_path, std::ios::binary).get();

//             msg.set_binary_message(file_input_stream);
//             // this->request_completions.insert(std::pair(std::string(file_path), request_status_t{}));
//             // Send the previously read file to the websocket remote endpoint.
//             this->client.send(msg);
//             this->setStatus(Status::TRANSCRIPTING);
//         }
//         catch (std::exception &e)
//         {
//             std::cout << e.what() << std::endl;
//         }
//     }

//     void Transcriptor::connect()
//     {
//         this->client.connect(U(this->url)).wait();
//         this->setStatus(Status::CONNECTING);
//         // this->running = true;
//     }

//     void Transcriptor::disconnect()
//     {
//         WS_OutMessage closeMsg;

//         closeMsg.set_utf8_message("EOS");
//         std::cout << "Sending EOS to disconnect..." << std::endl;
//         this->client.send(closeMsg);
//         this->setStatus(Status::DISCONNECTING);
//     }

//     Transcriptor &Transcriptor::operator=(const Transcriptor &other)
//     {
//         // Self assignment guard
//         if (this == &other)
//         {
//             return *this;
//         }

//         this->accessToken = other.accessToken;
//         this->client = other.client;
//         this->url = other.url;

//         return *this;
//     }
// }