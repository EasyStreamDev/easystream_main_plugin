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
#include "../server/include/interface/IServer.hpp"
#include "../utils/Utils.hpp"

// Local
#include "autoAudioLeveler/AutoAudioLeveler.hpp"
#include "profile/types/Scene.hpp"

// Linked
#include <unordered_map>
#include <memory>

namespace es::obs
{
    using Scene = types::Scene;

    static const std::vector<std::string> UNV_KINDS_AUDIO_IO = {"pulse_input_capture", "pulse_output_capture", "alsa_input_capture"};
    static const std::vector<std::string> UNV_KINDS_TEXT_FIELDS = {"text_ft2_source", "text_gdiplus"};
    static const std::vector<std::string> UNV_KINDS_DISPLAY_SOURCES = {
        "v4l2_input",
        "pipewire-desktop-capture-source",
        "xshm_input",
        "scene",
        "window_capture"
    };

    template <typename T>
    const bool vector_contains(const std::vector<T> &vec, const T &elem)
    {
        return bool(std::find(vec.begin(), vec.end(), elem) != vec.end());
    }

    class SourceTracker
    {
    public:
        SourceTracker();
        ~SourceTracker();

        void init(IPluginManager *);
        static bool filterSources(std::string name, obs_source_t *source);
        static bool filterAudioSources(std::string name, obs_source_t *source);
        static bool filterVideoSources(std::string name, obs_source_t *source);
        static bool filterTransitions(std::string name, obs_source_t *source);
        static bool filterScenes(std::string name, obs_source_t *source);
        const std::unordered_map<std::string, std::shared_ptr<AutoAudioLeveler>> &getAudioMap() const;
        const std::unordered_map<std::string, Scene *> &getSceneMap() const;
        const std::unordered_map<std::string, json> &getTextFieldMap() const;
        const std::unordered_map<std::string, json> &getDisplaySourcesMap() const;

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

        // Server related
        void submitToBroadcast(const json &);

    private:
        IPluginManager *m_PluginManager = nullptr;
        std::map<std::string, std::shared_ptr<obs_weak_source_t>> _sources;
        bool _obsLoaded;
        // std::vector<AutoAudioLeveler> _audioSource;
        std::unordered_map<std::string, std::shared_ptr<AutoAudioLeveler>> _audioLevelers;
        std::unordered_map<std::string, Scene *> _scenes;
        std::unordered_map<std::string, json> _textfields;
        std::unordered_map<std::string, json> _displaySources;
    };
}

#endif /* !SOURCETRACKER_HPP_ */