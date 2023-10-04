/*
** Easystream EIP, 2021
** easystream_main_plugin
** File description:
** eventsSceneItems
*/

#include "../SourceTracker.hpp"

void es::obs::SourceTracker::handleSceneItemCreated(void *param, calldata_t *data)
{
    obs_scene_t *scene = GetCalldataPointer<obs_scene_t>(data, "scene");
    obs_sceneitem_t *scene_item = GetCalldataPointer<obs_sceneitem_t>(data, "item");
    if (!scene || !scene_item)
    {
        return;
    }
    // Get source from scene item parent scene
    obs_source_t *scene_source = obs_scene_get_source(scene);
    // Get source from obs_sceneitem_t to gather data from it.
    obs_source_t *scene_item_source = obs_sceneitem_get_source(scene_item);
    // Reinstantiate source tracker from param.
    SourceTracker *source_tracker = (SourceTracker *)param;
    // Gather data about created scene item.
    std::string unv_kind(obs_source_get_unversioned_id(scene_item_source));

    // Check if item is a text field
    // @note : "text_ft2_source" kind is soon going to be deprecated.
    if (vector_contains(UNV_KINDS_TEXT_FIELDS, unv_kind))
    {
        // Add text field to the corresponding map in the source tracker.
        source_tracker->_textfields[obs_source_get_uuid(scene_item_source)] = json({
            {"parent_scene", obs_source_get_name(scene_source)},
            {"uuid", obs_source_get_uuid(scene_item_source)},
            {"name", obs_source_get_name(scene_item_source)},
        });
    }
    // Check if item is a display source
    else if (vector_contains(UNV_KINDS_DISPLAY_SOURCES, unv_kind))
    {
        source_tracker->_displaySources[obs_source_get_uuid(scene_item_source)] = json({
            {"parent_scene", obs_source_get_name(scene_source)},
            {"uuid", obs_source_get_uuid(scene_item_source)},
            {"name", obs_source_get_name(scene_item_source)},
        });
    }
}

void es::obs::SourceTracker::handleSceneItemRemoved(void *param, calldata_t *data)
{
    obs_scene_t *scene = GetCalldataPointer<obs_scene_t>(data, "scene");
    obs_sceneitem_t *scene_item = GetCalldataPointer<obs_sceneitem_t>(data, "item");
    if (!scene || !scene_item)
    {
        return;
    }
    // Get source from obs_sceneitem_t to gather data from it.
    obs_source_t *scene_item_source = obs_sceneitem_get_source(scene_item);
    // Reinstantiate source tracker from param.
    SourceTracker *source_tracker = (SourceTracker *)param;
    // Gather data about created scene item.
    std::string unv_kind(obs_source_get_unversioned_id(scene_item_source));

    // Check if item is a text field
    // @note : "text_ft2_source" kind is soon going to be deprecated.
    if (vector_contains(UNV_KINDS_TEXT_FIELDS, unv_kind))
    {
        // Erase text field corresponding to the uuid in the map.
        source_tracker->_textfields.erase(obs_source_get_uuid(scene_item_source));
    }
    else if (vector_contains(UNV_KINDS_DISPLAY_SOURCES, unv_kind))
    {
        // Erase display source corresponding to the uuid in the map.
        source_tracker->_displaySources.erase(obs_source_get_uuid(scene_item_source));
    }
}

void es::obs::SourceTracker::handleSceneItemListReindexed(void *param, calldata_t *data)
{
    obs_scene_t *scene = GetCalldataPointer<obs_scene_t>(data, "scene");
    obs_sceneitem_t *sceneItem = GetCalldataPointer<obs_sceneitem_t>(data, "item");
    if (!scene || !sceneItem)
    {
        return;
    }

    std::string name(obs_source_get_name(obs_scene_get_source(scene)));
    std::string itemName(obs_source_get_name(obs_sceneitem_get_source(sceneItem)));
    int sceneItemIndex = obs_sceneitem_get_order_position(sceneItem);
}

void es::obs::SourceTracker::handleSceneItemEnableStateChanged(void *param, calldata_t *data)
{
    obs_scene_t *scene = GetCalldataPointer<obs_scene_t>(data, "scene");
    obs_sceneitem_t *sceneItem = GetCalldataPointer<obs_sceneitem_t>(data, "item");
    if (!scene || !sceneItem)
    {
        return;
    }

    bool sceneItemEnabled = calldata_bool(data, "visible");

    std::string name(obs_source_get_name(obs_scene_get_source(scene)));
    int64_t sceneItemId = obs_sceneitem_get_id(sceneItem);
    // blog(LOG_INFO, "### [SourceTracker::handleSceneItemEnableStateChanged]: %s", name.c_str());
}

void es::obs::SourceTracker::handleSceneItemLockStateChanged(void *param, calldata_t *data)
{
    obs_scene_t *scene = GetCalldataPointer<obs_scene_t>(data, "scene");
    obs_sceneitem_t *sceneItem = GetCalldataPointer<obs_sceneitem_t>(data, "item");
    if (!scene || !sceneItem)
    {
        return;
    }

    bool sceneItemLocked = calldata_bool(data, "locked");

    std::string name(obs_source_get_name(obs_scene_get_source(scene)));
    int64_t sceneItemId = obs_sceneitem_get_id(sceneItem);
    // blog(LOG_INFO, "### [SourceTracker::handleSceneItemLockStateChanged]: %s", name.c_str());
}

void es::obs::SourceTracker::handleSceneItemTransformChanged(void *param, calldata_t *data)
{
    obs_scene_t *scene = GetCalldataPointer<obs_scene_t>(data, "scene");
    obs_sceneitem_t *sceneItem = GetCalldataPointer<obs_sceneitem_t>(data, "item");
    if (!scene || !sceneItem)
    {
        return;
    }

    std::string name(obs_source_get_name(obs_scene_get_source(scene)));
    int64_t sceneItemId = obs_sceneitem_get_id(sceneItem);
    // blog(LOG_INFO, "### [SourceTracker::handleMediaInputPlaybackEnded]: %s", name.c_str());
}