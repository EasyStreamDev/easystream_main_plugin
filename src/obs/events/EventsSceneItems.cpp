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
    if (!scene)
    {
        return;
    }
    obs_sceneitem_t *scene_item = GetCalldataPointer<obs_sceneitem_t>(data, "item");
    if (!scene_item)
    {
        return;
    }
    obs_source_t *scene_item_source = obs_sceneitem_get_source(scene_item);

    // Reinstantiate source tracker from param.
    SourceTracker *source_tracker = (SourceTracker *)param;
    // Gather data about created scene item.
    std::string parent_scene_name(obs_source_get_name(obs_scene_get_source(scene)));
    std::string item_name(obs_source_get_name(scene_item_source));
    std::string unversioned_kind(obs_source_get_unversioned_id(scene_item_source));

    { // @debug
      //   std::cerr << "Unversioned kind: " << unversioned_kind << std::endl;
      // blog(
      //     LOG_INFO,
      //     "### [SourceTracker::handleSceneItemCreated]\n\t%s::%s\n---",
      //     parent_scene_name.c_str(), item_name.c_str());
    }

    // Check if text field
    if (unversioned_kind == "text_ft2_source")
    {
        // Add text field to the corresponding map in the source tracker.
        source_tracker->_textfields[obs_source_get_uuid(scene_item_source)] = json({
            {"parent_scene", parent_scene_name},
            {"text_field", item_name},
        });
    }
}

void es::obs::SourceTracker::handleSceneItemRemoved(void *param, calldata_t *data)
{
    obs_scene_t *scene = GetCalldataPointer<obs_scene_t>(data, "scene");
    if (!scene)
    {
        return;
    }
    obs_sceneitem_t *scene_item = GetCalldataPointer<obs_sceneitem_t>(data, "item");
    if (!scene_item)
    {
        return;
    }
    obs_source_t *scene_item_source = obs_sceneitem_get_source(scene_item);

    SourceTracker *source_tracker = (SourceTracker *)param;
    std::string kind(obs_source_get_id(scene_item_source));
    std::string unversioned_kind(obs_source_get_unversioned_id(scene_item_source));

    { // @debug
      // blog(LOG_INFO, "### [SourceTracker::handleSceneItemRemoved]: %s", scene_name.c_str());
    }

    // Check if text field
    if (unversioned_kind == "text_ft2_source")
    {
        // Erase text field corresponding to the uuid in the map.
        source_tracker->_textfields.erase(obs_source_get_uuid(scene_item_source));
    }
}

void es::obs::SourceTracker::handleSceneItemListReindexed(void *param, calldata_t *data)
{
    obs_scene_t *scene = GetCalldataPointer<obs_scene_t>(data, "scene");
    if (!scene)
    {
        return;
    }
    obs_sceneitem_t *sceneItem = GetCalldataPointer<obs_sceneitem_t>(data, "item");
    if (!sceneItem)
    {
        return;
    }

    std::string name(obs_source_get_name(obs_scene_get_source(scene)));
    std::string itemName(obs_source_get_name(obs_sceneitem_get_source(sceneItem)));
    int sceneItemIndex = obs_sceneitem_get_order_position(sceneItem);

    // blog(LOG_INFO, "### [SourceTracker::handleSceneItemListReindexed]: %s", name.c_str());
}

void es::obs::SourceTracker::handleSceneItemEnableStateChanged(void *param, calldata_t *data)
{
    obs_scene_t *scene = GetCalldataPointer<obs_scene_t>(data, "scene");
    if (!scene)
    {
        return;
    }
    obs_sceneitem_t *sceneItem = GetCalldataPointer<obs_sceneitem_t>(data, "item");
    if (!sceneItem)
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
    if (!scene)
        return;

    obs_sceneitem_t *sceneItem = GetCalldataPointer<obs_sceneitem_t>(data, "item");
    if (!sceneItem)
        return;

    bool sceneItemLocked = calldata_bool(data, "locked");

    std::string name(obs_source_get_name(obs_scene_get_source(scene)));
    int64_t sceneItemId = obs_sceneitem_get_id(sceneItem);
    // blog(LOG_INFO, "### [SourceTracker::handleSceneItemLockStateChanged]: %s", name.c_str());
}

void es::obs::SourceTracker::handleSceneItemTransformChanged(void *param, calldata_t *data)
{
    obs_scene_t *scene = GetCalldataPointer<obs_scene_t>(data, "scene");
    if (!scene)
        return;

    obs_sceneitem_t *sceneItem = GetCalldataPointer<obs_sceneitem_t>(data, "item");
    if (!sceneItem)
        return;

    std::string name(obs_source_get_name(obs_scene_get_source(scene)));
    int64_t sceneItemId = obs_sceneitem_get_id(sceneItem);
    // blog(LOG_INFO, "### [SourceTracker::handleMediaInputPlaybackEnded]: %s", name.c_str());
}