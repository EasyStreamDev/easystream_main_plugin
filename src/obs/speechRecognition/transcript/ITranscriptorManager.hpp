#ifndef ITRANSCRIPTOR_MANAGER_HPP
#define ITRANSCRIPTOR_MANAGER_HPP

#include <string>
#include <vector>
#include <memory>

namespace es::transcription
{
    typedef struct transcription_result_s
    {
        uint id;
        std::string file_path;
        std::vector<std::string> transcription;
    } ts_result_t;

    class ITranscriptorManager
    {
    public:
        virtual void storeTranscription(const ts_result_t &) = 0;
    };
}

#endif // ITRANSCRIPTOR_MANAGER_HPP