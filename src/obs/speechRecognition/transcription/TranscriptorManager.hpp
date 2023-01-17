#ifndef TRANSCRIPTOR_MANAGER_HPP
#define TRANSCRIPTOR_MANAGER_HPP

#include "Transcriptor.hpp"

namespace es
{
    class TranscriptorManager
    {
    public:
        TranscriptorManager();
        ~TranscriptorManager();

        void start(void);
        void stop(void);

        void transcriptFile(const std::string &, Transcriptor::ResponseCallback);
        inline const bool &isRunning() { return this->running; }

    private:
        Transcriptor &getFreeTranscriptor();

    private:
        std::string accessToken;
        std::array<Transcriptor, 4> transcriptors;
        std::vector<std::string> filesToTranscript;
        bool running = false;
    };
}

#endif // TRANSCRIPTOR_MANAGER_HPP