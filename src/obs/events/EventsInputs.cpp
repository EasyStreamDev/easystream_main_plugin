/*
** easyStream EIP, 2021
** easystream_main_plugin
** File description:
** eventInputs
*/

#include "../SourceTracker.hpp"
#include "../autoAudioLeveler/AutoAudioLeveler.hpp"

void es::obs::SourceTracker::handleInputCreated(obs_source_t *source)
{
    const std::string name = obs_source_get_name(source);
    const std::string uuid = obs_source_get_uuid(source);
    const std::string kind = obs_source_get_id(source); // @warning : depends on sound card
    OBSDataAutoRelease inputSettings = obs_source_get_settings(source);
    OBSDataAutoRelease defaultInputSettings = obs_get_source_defaults(kind.c_str());

    if (!filterAudioSources("audio_input", source))
    {
        _audioLevelers.insert(
            std::pair<std::string, std::shared_ptr<AutoAudioLeveler>>(
                name,
                std::make_shared<AutoAudioLeveler>(source)));
        blog(LOG_INFO, "### Instancing Audio Leveler for %s", name.c_str());
    }

    { // @debug
      // blog(LOG_INFO, "### [SourceTracker::handleInputCreated]: %s:%s:%s", name.c_str(), kind.c_str(), uuid.c_str());
      // std::cerr << "Kind: " << kind << std::endl;
    }

    // @todo: map of string to func depending on "kind" var.

    if (
        kind == "pulse_input_capture" ||
        kind == "pulse_output_capture" ||
        kind == "alsa_input_capture")
    {
        const json broadcastRequestData = {
            {"type", "audioSourceCreated"},
            {"name", name},
            {"uuid", uuid},
            {"type", kind},
        };
        this->submitToBroadcast(broadcastRequestData);
    }
}

void es::obs::SourceTracker::handleInputRemoved(obs_source_t *source)
{
    const std::string name = obs_source_get_name(source);
    const std::string uuid = obs_source_get_uuid(source);
    const std::string kind = obs_source_get_id(source); // @warning : depends on sound card

    { // @debug
      // blog(LOG_INFO, "### [SourceTracker::handleInputRemoved]: %s ", name.c_str());
    }

    // @todo: map of string to func depending on "kind" var.

    if (
        kind == "pulse_input_capture" ||
        kind == "pulse_output_capture" ||
        kind == "alsa_input_capture")
    {
        const json broadcastRequestData = {
            {"type", "audioSourceRemoved"},
            {"name", name},
            {"uuid", uuid},
            {"type", kind},
        };
        this->submitToBroadcast(broadcastRequestData);
    }
}

void es::obs::SourceTracker::handleInputNameChanged(obs_source_t *source, std::string oldName, std::string name)
{
    const std::string uuid = obs_source_get_uuid(source);
    const std::string unv_kind = obs_source_get_unversioned_id(source); // @warning : depends on device hardware/OS

    { // @debug
      // blog(LOG_INFO, "### [SourceTracker::handleInputNameChanged]: from %s to %s.", oldName.c_str(), name.c_str());
      // std::vector<json> j = es::utils::obs::listHelper::GetSceneList();
    }

    if (
        unv_kind == "pulse_input_capture" ||
        unv_kind == "pulse_output_capture" ||
        unv_kind == "alsa_input_capture")
    {
        const json broadcastRequestData = {
            {"type", "audioSourceNameChanged"},
            {"name", name},
            {"oldName", oldName},
            {"uuid", uuid},
        };
        this->submitToBroadcast(broadcastRequestData);
    }
    else if (unv_kind == "text_ft2_source")
    {
        this->_textfields[uuid].at("text_field") = name;
    }
}

void es::obs::SourceTracker::handleInputActiveStateChanged(void *param, calldata_t *data)
{
    auto eventHandler = static_cast<es::obs::SourceTracker *>(param);

    obs_source_t *source = GetCalldataPointer<obs_source_t>(data, "source");
    if (!source)
        return;

    if (obs_source_get_type(source) != OBS_SOURCE_TYPE_INPUT)
        return;

    std::string name(obs_source_get_name(source));
    bool active = obs_source_active(source);
    // blog(LOG_INFO, "### [SourceTracker::handleInputActiveStateChanged]: %s", name.c_str());
}

void es::obs::SourceTracker::handleInputShowStateChanged(void *param, calldata_t *data)
{
    auto eventHandler = static_cast<es::obs::SourceTracker *>(param);

    obs_source_t *source = GetCalldataPointer<obs_source_t>(data, "source");
    if (!source)
        return;

    if (obs_source_get_type(source) != OBS_SOURCE_TYPE_INPUT)
        return;

    std::string name(obs_source_get_name(source));
    bool showing = obs_source_showing(source);
    // blog(LOG_INFO, "### [SourceTracker::handleInputShowStateChanged]: %s", name.c_str());
}

void es::obs::SourceTracker::handleInputMuteStateChanged(void *param, calldata_t *data)
{
    auto eventHandler = static_cast<es::obs::SourceTracker *>(param);

    obs_source_t *source = GetCalldataPointer<obs_source_t>(data, "source");
    if (!source)
        return;

    if (obs_source_get_type(source) != OBS_SOURCE_TYPE_INPUT)
        return;

    std::string name = obs_source_get_name(source);
    bool muted = obs_source_muted(source);
    // blog(LOG_INFO, "### [SourceTracker::handleInputMuteStateChanged]: %s", name.c_str());
}

void es::obs::SourceTracker::handleInputVolumeChanged(void *param, calldata_t *data)
{
    auto eventHandler = static_cast<es::obs::SourceTracker *>(param);

    obs_source_t *source = GetCalldataPointer<obs_source_t>(data, "source");
    if (!source)
        return;

    if (obs_source_get_type(source) != OBS_SOURCE_TYPE_INPUT)
        return;

    // Volume must be grabbed from the calldata. Running obs_source_get_volume() will return the previous value.
    double inputVolumeMul = calldata_float(data, "volume");

    double inputVolumeDb = obs_mul_to_db(inputVolumeMul);
    if (inputVolumeDb == -INFINITY)
        inputVolumeDb = -100;

    std::string name(obs_source_get_name(source));
    // blog(LOG_INFO, "### [SourceTracker::handleInputVolumeChanged]: %s ", name.c_str());
}

void es::obs::SourceTracker::handleInputAudioSyncOffsetChanged(void *param, calldata_t *data)
{
    auto eventHandler = static_cast<es::obs::SourceTracker *>(param);

    obs_source_t *source = GetCalldataPointer<obs_source_t>(data, "source");
    if (!source)
        return;

    if (obs_source_get_type(source) != OBS_SOURCE_TYPE_INPUT)
        return;

    long long inputAudioSyncOffset = calldata_int(data, "offset");

    std::string name(obs_source_get_name(source));
    long offset = inputAudioSyncOffset / 1000000;
    // blog(LOG_INFO, "### [SourceTracker::handleInputAudioSyncOffsetChanged]: %s ", name.c_str());
}

void es::obs::SourceTracker::handleInputAudioTracksChanged(void *param, calldata_t *data)
{
    auto eventHandler = static_cast<es::obs::SourceTracker *>(param);

    obs_source_t *source = GetCalldataPointer<obs_source_t>(data, "source");
    if (!source)
        return;

    if (obs_source_get_type(source) != OBS_SOURCE_TYPE_INPUT)
        return;

    std::string name(obs_source_get_name(source));
    // blog(LOG_INFO, "### [SourceTracker::handleInputAudioTracksChanged]: %s ", name.c_str());
}

void es::obs::SourceTracker::handleInputAudioMonitorTypeChanged(void *param, calldata_t *data)
{
    auto eventHandler = static_cast<es::obs::SourceTracker *>(param);

    obs_source_t *source = GetCalldataPointer<obs_source_t>(data, "source");
    if (!source)
        return;

    if (obs_source_get_type(source) != OBS_SOURCE_TYPE_INPUT)
        return;

    std::string name(obs_source_get_name(source));
    // blog(LOG_INFO, "### [SourceTracker::handleInputAudioMonitorTypeChanged]: %s ", name.c_str());
}