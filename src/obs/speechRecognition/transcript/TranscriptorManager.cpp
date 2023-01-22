#include "TranscriptorManager.hpp"

namespace es::transcription
{
    TranscriptorManager::TranscriptorManager()
    {
        // @dev YerimB : To get from env ?
        const std::string rev_ai_token = std::getenv("REVAI_TOKEN");

        if (rev_ai_token.empty())
        {
            throw "Rev.ai token not set.";
        }

        // this->accessToken = "02dsbSDq9R5nb9iHJrpgeNlplkRAJkvvWZh59WJfnVULUZqU9ovP-EIFUj1sbRtV0bJXFlbyfwd0Kg99Psap5x4gzLLaE"; // Get access token and store it
        this->accessToken = rev_ai_token;

        for (int i = 0; i < this->transcriptors.size(); ++i)
        {
            this->transcriptors[i].setAccessToken(this->accessToken);
        }
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
        for (auto &t : this->transcriptors)
        {
            t.start();
        }
        this->running = true;
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
        this->running = false;
    }

    void TranscriptorManager::transcriptFile(
        const std::string &path,
        Transcriptor::ResponseCallback cb_)
    {
        Transcriptor &t = this->getFreeTranscriptor();

        t.start();
        t.setResponseCallback(cb_);
        // Wait for connection to WS remote endpoint.
        while (t.getStatus() == Transcriptor::Status::CONNECTED)
            ;
        // Send audio file to transcript.
        t.sendAudio(path);
    }

    /***********/
    /* PRIVATE */
    /***********/

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
        }
    }
}