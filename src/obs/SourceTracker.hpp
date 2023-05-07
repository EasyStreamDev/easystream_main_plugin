/*
** EasyStream EIP, 2021
** easystream_main_plugin
** File description:
** sourceTracker
*/

#ifndef SOURCETRACKER_HPP_
#define SOURCETRACKER_HPP_

// Global
#include "../IPluginManager.hpp"
#include "../utils/Utils.hpp"

// Local
#include "autoAudioLeveler/AutoAudioLeveler.hpp"

// Linked
#include <unordered_map>
#include <memory>

namespace es::obs
{
    class SourceTracker
    {
    public:
        SourceTracker();
        ~SourceTracker();

        void init();
        static bool filterSources(std::string name, obs_source_t *source);
        static bool filterAudioSources(std::string name, obs_source_t *source);
        static bool filterVideoSources(std::string name, obs_source_t *source);
        static bool filterTransitions(std::string name, obs_source_t *source);
        static bool filterScenes(std::string name, obs_source_t *source);
        const std::unordered_map<std::string, std::shared_ptr<AutoAudioLeveler>> &getAudioMap() const;

    private:
        static void onFrontendEvent(enum obs_frontend_event event, void *private_data);

        static void createHandler(void *ptr, calldata_t *data);
        static void destroyHandler(void *ptr, calldata_t *data);
        static void renameHandler(void *ptr, calldata_t *data);

        typedef std::function<bool(std::string, obs_source_t *)> enumerate_cb_t;

        typedef std::function<bool(std::string, obs_source_t *)> filter_cb_t;

        void enumerate(enumerate_cb_t enumerate_cb, filter_cb_t filter_cb = nullptr);

        void connectSourceSignals(obs_source_t *source);
        void disconnectSourceSignals(obs_source_t *source);

        // Signal handler: libobs
        static void sourceCreatedMultiHandler(void *param, calldata_t *data);
        static void sourceDestroyedMultiHandler(void *param, calldata_t *data);
        static void sourceRemovedMultiHandler(void *param, calldata_t *data);

        // Signal handler: source
        static void sourceRenamedMultiHandler(void *param, calldata_t *data);
        static void sourceMediaPauseMultiHandler(void *param, calldata_t *data);
        static void sourceMediaPlayMultiHandler(void *param, calldata_t *data);
        static void sourceMediaRestartMultiHandler(void *param, calldata_t *data);
        static void sourceMediaStopMultiHandler(void *param, calldata_t *data);
        static void sourceMediaNextMultiHandler(void *param, calldata_t *data);
        static void sourceMediaPreviousMultiHandler(void *param, calldata_t *data);

        // Scenes
        /* Callback responding to: Scene creation. */
        void handleSceneCreated(obs_source_t *source);
        /* Callback responding to: Scene removal. */
        void handleSceneRemoved(obs_source_t *source);
        /* Callback responding to: Scene name change. */
        void handleSceneNameChanged(obs_source_t *source, std::string oldSceneName, std::string sceneName);
        void handleCurrentSceneChanged();
        void handleCurrentPreviewSceneChanged();
        void handleSceneListChanged();

        // Inputs
        /* Callback responding to: Input/Output audio source creation. */
        void handleInputCreated(obs_source_t *source);
        /* Callback responding to: Input/Output audio source removal. */
        void handleInputRemoved(obs_source_t *source);
        /* Callback responding to: Input/Output audio source name change. */
        void handleInputNameChanged(obs_source_t *source, std::string oldInputName, std::string inputName);
        static void handleInputActiveStateChanged(void *param, calldata_t *data);
        static void handleInputShowStateChanged(void *param, calldata_t *data);
        static void handleInputMuteStateChanged(void *param, calldata_t *data);
        static void handleInputVolumeChanged(void *param, calldata_t *data);
        static void handleInputAudioSyncOffsetChanged(void *param, calldata_t *data);
        static void handleInputAudioTracksChanged(void *param, calldata_t *data);
        static void handleInputAudioMonitorTypeChanged(void *param, calldata_t *data);

        // Transitions
        void handleTransitionCreated(obs_source_t *source);
        void handleTransitionRemoved(obs_source_t *source);
        void handleTransitionNameChanged(obs_source_t *source, std::string oldTransitionName, std::string transitionName);

        // Scene Items
        static void handleSceneItemCreated(void *param, calldata_t *data);
        static void handleSceneItemRemoved(void *param, calldata_t *data);
        static void handleSceneItemListReindexed(void *param, calldata_t *data);
        static void handleSceneItemEnableStateChanged(void *param, calldata_t *data);
        static void handleSceneItemLockStateChanged(void *param, calldata_t *data);
        static void handleSceneItemTransformChanged(void *param, calldata_t *data);

        // Media Inputs
        static void handleMediaInputPlaybackStarted(void *param, calldata_t *data);
        static void handleMediaInputPlaybackEnded(void *param, calldata_t *data);

    private:
        es::server::AsioTcpServer *m_TcpServer = nullptr;
        std::map<std::string, std::shared_ptr<obs_weak_source_t>> _sources;
        bool _obsLoaded;
        // std::vector<AutoAudioLeveler> _audioSource;
        std::unordered_map<std::string, std::shared_ptr<AutoAudioLeveler>> _audioLevelers;
    };
}

#endif /* !SOURCETRACKER_HPP_ */