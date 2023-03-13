#include "TranscriptorManager.hpp"

namespace es::transcription
{
    TranscriptorManager::TranscriptorManager()
    {
        for (int idx = 0; idx < m_Transcriptors.size(); ++idx)
        {
            m_Transcriptors[idx] = Transcriptor(this);
        }

        // @dev YerimB : To get from env ?
        // const std::string rev_ai_token = std::getenv("REVAI_TOKEN");

        // if (rev_ai_token.empty())
        // {
        //     throw "Rev.ai token not set.";
        // }

        // this->accessToken = rev_ai_token;
        this->accessToken = "02Pp5jWvtnGyLEOpiFYkFUXHzXGFh_Uc-FGTLWsSg94VZwzTXdBlOtgsaAa-KqXA7_DeycShbVpQM2cdroLBrxrOKRG78"; // Get access token and store it
    }

    TranscriptorManager::~TranscriptorManager()
    {
        this->stop();
    }

    /**********/
    /* PUBLIC */
    /**********/

    void TranscriptorManager::run(void *)
    {
        while (1) // @todo : setup a boolean thread killer
        {
            m_FilesQueueMutex.lock();
            while (!m_FilesQueue.empty())
            {
                blog(LOG_INFO, "---------- TranscriptorManager : treating file...");
                // Get next file to transcript
                auto &data_ = m_FilesQueue.front();

                // Submit file to transcriptor, returns true if success.
                if (this->transcriptFile(data_.first, data_.second))
                {
                    blog(LOG_INFO, "---------- TranscriptorManager : Success - file was submitted for transcription.");
                    // Remove file from queue if succesfully submitted.
                    m_FilesQueue.pop();
                }
                else // If file could not be submitted (i.e. no transcriptor is available).
                {
                    break;
                }
            }
            m_FilesQueueMutex.unlock();
            this->thread_sleep_ms(1000); // Not to run fullspeed.
            blog(LOG_INFO, "---------- TranscriptorManager : Still running...");
        }
    }

    const uint TranscriptorManager::submit(const std::string &file_path)
    {
        // Get a unique transcription ID (@todo @maybe : instead of using uint type, use a real UUID)
        const uint transcription_id = this->getNewTranscriptionId();

        blog(LOG_INFO, "---------- TranscriptorManager : Waiting for lock to free...");
        // Thread protected add of file to transcript in the queue.
        m_FilesQueueMutex.lock();
        m_FilesQueue.push(Pair<uint, String>(transcription_id, file_path));
        m_FilesQueueMutex.unlock();

        blog(LOG_INFO, "---------- TranscriptorManager : File submitted.");
        return transcription_id;
    }

    void TranscriptorManager::stop(void)
    {
        // Tell transcriptors to disconnect and stop their work.
        for (auto &t : m_Transcriptors)
        {
            t.stop();
        }
        // Check for disconnection.
        for (auto &t : m_Transcriptors)
        {
            while (t.getStatus() != Transcriptor::Status::DISCONNECTED)
            {
                // Prevent running at processor fullspeed.
                this->thread_sleep_ms(1);
            }
        }
    }

    void TranscriptorManager::storeTranscription(const ts_result_t &result_)
    {
        m_ResultsMutex.lock();
        m_Results[result_.id] = result_;
        m_ResultsMutex.unlock();
    }

    /***********/
    /* PRIVATE */
    /***********/

    const bool TranscriptorManager::transcriptFile(
        const uint &id,
        const std::string &path)
    {
        // Try getting free transcriptor
        Transcriptor *t = this->getFreeTranscriptor();

        // Check if any transcriptor was available.
        if (!t)
        {
            // Returns false if no transcriptor are available
            return false;
        }

        blog(LOG_INFO, "---------- TranscriptorManager : Free Transcriptor found.");

        // Initialize transcriptor with access token.
        t->init(this->accessToken);
        // Initiate connection to WS remote endpoint.
        blog(LOG_INFO, "---------- TranscriptorManager : Connecting to remote endpoint...");
        t->start();
        // Wait for connection completion.
        while (t->getStatus() != Transcriptor::Status::CONNECTED)
        {
            // Prevent running at processor fullspeed.
            this->thread_sleep_ms(5);
        }
        blog(LOG_INFO, "---------- TranscriptorManager : Connection established.");
        // Transcriptor connected : sending audio file to transcript.
        t->sendAudio(id, path);

        // Returns true if file was succesfully submitted to transcriptor.
        return true;
    }

    Transcriptor *TranscriptorManager::getFreeTranscriptor()
    {
        for (auto &t : m_Transcriptors)
        {
            if (t.getStatus() == Transcriptor::Status::DISCONNECTED)
            {
                return &t;
            }
        }

        // No transcriptor available
        return nullptr;
    }

    const uint TranscriptorManager::getNewTranscriptionId(void) const
    {
        static uint TRANSCRIPTION_ID = 0;

        return ++TRANSCRIPTION_ID;
    }
}