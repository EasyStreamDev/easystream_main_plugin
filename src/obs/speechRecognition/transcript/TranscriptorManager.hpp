// #ifndef TRANSCRIPTOR_MANAGER_HPP
// #define TRANSCRIPTOR_MANAGER_HPP

// #include "Transcriptor.hpp"
// #include "../../../Runnable.hpp"

// namespace es::transcription
// {
//     class TranscriptorManager : public es::Runnable
//     {
//     public:
//         TranscriptorManager();
//         ~TranscriptorManager();

//         // void start(void);
//         void run(void *) override;
//         void stop(void);

//     private:
//         void transcriptFile(const std::string &, Transcriptor::ResponseCallback);
//         Transcriptor &getFreeTranscriptor();

//     private:
//         std::string accessToken;
//         std::array<Transcriptor, 4> transcriptors;
//         Queue<Pair<String, Transcriptor::ResponseCallback>> filesToTranscript;
//     };
// }

// #endif // TRANSCRIPTOR_MANAGER_HPP