#ifndef TRANSCRIPTOR_MANAGER_HPP
#define TRANSCRIPTOR_MANAGER_HPP

#include "ITranscriptorManager.hpp"
#include "Transcriptor.hpp"
#include "../../../Runnable.hpp"
#include "../../../IPluginManager.hpp"

namespace es::transcription
{
    class TranscriptorManager : public es::Runnable, ITranscriptorManager
    {
    public:
        const static int INACTIVITY_TIMEOUT_MS = 10000;

    public:
        TranscriptorManager(const std::function<void(std::vector<std::string>)> &);
        ~TranscriptorManager();

        // void start(void);
        void init(IPluginManager *);
        void run(void *) override;
        const uint submit(const std::string &);
        void stop(void);
        void storeTranscription(const ts_result_t &);
        const Optional<ts_result_t> getTranscription(const int & = -1);

    private:
        const bool transcriptFile(const uint &, const std::string &);
        Transcriptor *getFreeTranscriptor();
        const uint getNewTranscriptionId(void) const;

    private:
        std::string accessToken;
        std::array<Transcriptor, 5> m_Transcriptors;
        // Files queue (to be transcripted)
        Queue<Pair<uint, String>> m_FilesQueue;
        std::mutex m_FilesQueueMutex;
        // Transcription results (transcripted)
        // @todo (yem): make vector of pair instead. why ?
        Umap<uint, ts_result_t> m_Results;
        std::mutex m_ResultsMutex;
        std::function<void(std::vector<std::string>)> _pushToArea;

        // Plugin manager
        IPluginManager *m_PluginManager = nullptr;
    };
}

#endif // TRANSCRIPTOR_MANAGER_HPP