#include "Transcriptor.hpp"

namespace es::transcription
{
    Transcriptor::Transcriptor(ITranscriptorManager *tsManager_)
        : m_TSManager(tsManager_)
    {
    }

    Transcriptor::~Transcriptor()
    {
        this->stop();
    }

    void Transcriptor::init(const std::string &aToken_)
    {
        this->setAccessToken(aToken_);

        // Setup WS client
        this->client = WS_CallbackClient();
        // Message received from remote WS endpoint callback
        this->client.set_message_handler(
            [&](ws_socket::websocket_incoming_message msg)
            {
                try
                {
                    json data = json::parse(msg.extract_string().get());
                    const auto &response_type = std::string(data["type"]);

                    if (response_type == "connected")
                    {
                        // blog(LOG_INFO, "--------- WSCallback : WebSocket Connected.");
                        this->setStatus(Status::CONNECTED);
                    }
                    else // if (response_type == "final" || response_type == "partial")
                    {
                        ITranscriptorManager *tm = this->getManager();

                        if (tm != nullptr)
                        {
                            const Vector<json> &elements = data.at("elements");
                            Vector<String> transcript;

                            for (const json &elem : elements)
                            {
                                transcript.push_back(elem.at("value"));
                            }
                            this->m_FileData.transcription = transcript;
                            tm->storeTranscription(this->m_FileData);
                        }
                        else
                        {
                            std::cerr << "[Transcriptor] - Transcriptor manager not found." << std::endl;
                        }

                        // When final response received, send disconnect message.
                        if (response_type == "final")
                        {
                            this->stop();
                        }
                    }
                }
                catch (std::exception e)
                {
                    std::cerr << "[Transcriptor] - Callback raised an exception: " << e.what() << std::endl;
                }
            });
        // Set disconnect confirmation callback.
        this->client.set_close_handler(
            [&](
                ws_socket::websocket_close_status close_status,
                const utility::string_t &reason,
                const std::error_code &error)
            {
                // std::cout << "Closing Connection... " << std::flush;
                client.close();        // Close connection to remote WS.
                this->m_FileData = {}; // Reset current file data.
                this->setStatus(Status::DISCONNECTED);
                // std::cout << "Connection closed." << std::endl;
            });
    }

    void Transcriptor::start()
    {
        // Connect to the WS endpoint.
        this->connect();
    }

    void Transcriptor::stop()
    {
        if (this->status != Status::DISCONNECTING &&
            this->status != Status::DISCONNECTED)
        {
            this->disconnect();
        }
    }

    void Transcriptor::setAccessToken(const std::string &at_)
    {
        this->accessToken = at_;
        this->url = this->websocketEndpoint + "?access_token=" +
                    this->accessToken + this->contentTypeWAV;
    }

    void Transcriptor::sendAudio(const uint &id, const std::string &file_path)
    {
        // Store file transcription data
        m_FileData.id = id;
        m_FileData.file_path = file_path;

        try
        {
            WS_OutMessage msg;
            concurrency::streams::file_stream<uint8_t> buffer;

            // buffer.open_istream is asynchronous (returns a task). Using .get() on it
            // will wait for the task to be fully executed and return the result which here is
            // an input file stream (type: Concurrency::streams::istream)
            auto file_input_stream = buffer.open_istream(utility::conversions::to_string_t(file_path), std::ios::binary).get();

            msg.set_binary_message(file_input_stream);
            // this->request_completions.insert(std::pair(std::string(file_path), request_status_t{}));
            // Send the previously read file to the websocket remote endpoint.
            this->client.send(msg);
            this->setStatus(Status::TRANSCRIPTING);
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    void Transcriptor::connect()
    {
        this->client.connect(utility::conversions::to_string_t(this->url)).wait();
        this->setStatus(Status::CONNECTING);
    }

    void Transcriptor::disconnect()
    {
        WS_OutMessage closeMsg;

        closeMsg.set_utf8_message("EOS");
        // std::cout << "Sending EOS to disconnect..." << std::endl;
        this->client.send(closeMsg);
        // std::cout << "EOS sent !" << std::endl;
        this->setStatus(Status::DISCONNECTING);
    }

    Transcriptor &Transcriptor::operator=(const Transcriptor &other)
    {
        // Self assignment guard
        if (this == &other)
        {
            return *this;
        }

        this->accessToken = other.accessToken;
        this->client = other.client;
        this->url = other.url;
        this->m_TSManager = other.m_TSManager;

        return *this;
    }
}