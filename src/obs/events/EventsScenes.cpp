/*
** EasyStream EIP, 2021
** easystream_main_plugin
** File description:
** eventScenes
*/

#include "../SourceTracker.hpp"

void es::obs::SourceTracker::handleSceneCreated(obs_source_t *source)
{
    // bool group = obs_source_is_group(source);
    std::string name(obs_source_get_name(source));
    std::string uuid(obs_source_get_uuid(source));

    blog(LOG_INFO, "### [SourceTracker::handleSceneCreated] %s:%s", name.c_str(), uuid.c_str());

    // @todo: submit to server
    const json broadcastRequestData = {
        {"type", "sceneCreated"},
        {"name", name},
        {"uuid", uuid},
    };
    this->submitToBroadcast(broadcastRequestData);
}

void es::obs::SourceTracker::handleSceneRemoved(obs_source_t *source)
{
    // bool group = obs_source_is_group(source);
    std::string name(obs_source_get_name(source));
    std::string uuid(obs_source_get_uuid(source));

    blog(LOG_INFO, "### [SourceTracker::handleSceneRemoved]: %s:%s", name.c_str(), uuid.c_str());

    // @todo: submit to server
    const json broadcastRequestData = {
        {"type", "sceneRemoved"},
        {"name", name},
        {"uuid", uuid},
    };
    this->submitToBroadcast(broadcastRequestData);
}

void es::obs::SourceTracker::handleSceneNameChanged(obs_source_t *source, std::string oldName, std::string name)
{
    std::string uuid(obs_source_get_uuid(source));

    blog(LOG_INFO, "### [SourceTracker::handleSceneNameChanged]: %s:%s", name.c_str(), uuid.c_str());

    // @todo: submit to server
    const json broadcastRequestData = {
        {"type", "sceneNameChanged"},
        {"name", name},
        {"oldName", oldName},
        {"uuid", uuid},
    };
    this->submitToBroadcast(broadcastRequestData);
}

void es::obs::SourceTracker::handleCurrentSceneChanged()
{
    obs_source_t *currentScene = obs_frontend_get_current_scene();

    std::string name(obs_source_get_name(currentScene));

    blog(LOG_INFO, "### [SourceTracker::handleCurrentSceneChanged]: %s", name.c_str());
}

void es::obs::SourceTracker::handleCurrentPreviewSceneChanged()
{
    obs_source_t *currentPreviewScene = obs_frontend_get_current_preview_scene();

    if (!currentPreviewScene)
        return;

    std::string name(obs_source_get_name(currentPreviewScene));

    blog(LOG_INFO, "### [SourceTracker::handleCurrentPreviewSceneChanged]: %s", name.c_str());
}

void es::obs::SourceTracker::handleSceneListChanged()
{
    blog(LOG_INFO, "### [SourceTracker::handleSceneListChanged]:");
}