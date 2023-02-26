#include "TranscriptorManager.hpp"

namespace es::transcription
{
    TranscriptorManager::TranscriptorManager()
    {
        // @dev YerimB : To get from env ?
        // const std::string rev_ai_token = std::getenv("REVAI_TOKEN");

        // if (rev_ai_token.empty())
        // {
        //     throw "Rev.ai token not set.";
        // }

        // this->accessToken = rev_ai_token;
        this->accessToken = "02dsbSDq9R5nb9iHJrpgeNlplkRAJkvvWZh59WJfnVULUZqU9ovP-EIFUj1sbRtV0bJXFlbyfwd0Kg99Psap5x4gzLLaE"; // Get access token and store it
    }

    TranscriptorManager::~TranscriptorManager()
    {
        this->stop();
    }

    /**********/
    /* PUBLIC */
    /**********/

    void TranscriptorManager::start(void)
    {
        for (int i = 0; i < this->transcriptors.size(); ++i)
        {
            this->transcriptors[i].setAccessToken(this->accessToken);
        }
    }

    void TranscriptorManager::run(void *)
    {

        while (1)
        {
            // @test
            // @todo : repair crash on reconnecting previously connected socket.
            blog(LOG_INFO, "--------- Transcriptor manager running : Submitted file.");
            String prefix = "/home/yem/delivery/Epitech/EIP/easystream_main_plugin/Tests/ressources/";
            this->thread_sleep_ms(5000);
            this->filesToTranscript.push(
                Pair<
                    String,
                    Transcriptor::ResponseCallback>(
                    prefix + "league.wav",
                    [](const json &data)
                    {
                        std::string buf;

                        if (data["type"] != "final")
                        {
                            return;
                        }
                        auto &elems = data["elements"];
                        for (auto &elem : elems)
                        {
                            buf += elem["value"];
                        }
                        buf += "\n";
                        blog(LOG_INFO, "----------- Result : %s", buf.c_str());
                    }));
            // @endtest
            while (!this->filesToTranscript.empty())
            {
                auto &data_ = this->filesToTranscript.front();

                this->transcriptFile(data_.first, data_.second);

                this->filesToTranscript.pop();
            }
            blog(LOG_INFO, "--------- Transcriptor manager running : Nothing left to transcript.");
            this->thread_sleep_ms(10000); // Not to run fullspeed.
        }
    }

    void TranscriptorManager::stop(void)
    {
        for (auto &t : this->transcriptors)
        {
            t.stop();
        }
        for (auto &t : this->transcriptors)
        {
            while (t.getStatus() != Transcriptor::Status::DISCONNECTED)
                ;
        }
    }

    /***********/
    /* PRIVATE */
    /***********/

    void TranscriptorManager::transcriptFile(
        const std::string &path,
        Transcriptor::ResponseCallback cb_)
    {
        Transcriptor &t = this->getFreeTranscriptor();

        t.start();
        t.setResponseCallback(cb_);
        // Wait for connection to WS remote endpoint.
        while (t.getStatus() != Transcriptor::Status::CONNECTED)
        {
            this->thread_sleep_ms(5);
        }
        // Send audio file to transcript.
        t.sendAudio(path);
    }

    Transcriptor &TranscriptorManager::getFreeTranscriptor()
    {
        while (1)
        {
            for (auto &t : this->transcriptors)
            {
                if (t.getStatus() == Transcriptor::Status::DISCONNECTED)
                {
                    return t;
                }
            }
            this->thread_sleep_ms(5);
        }
    }
}