/*
obs-text-slideshow
Copyright (C) 2021 Joshua Wong jbwong05@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <obs-module.h>
#include <iostream>
#include <string>
#include <vector>

#include "plugin-macros.generated.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

#define CLAMP(x, min, max) ((x) < min ? min : ((x) > max ? max : (x)))

struct InputInfo
{
	std::string name;
	std::string id;
	std::string type;
	std::string description;
	std::string value;
};

struct EnumInputInfo
{
	std::string inputKind;
	std::vector<InputInfo> inputs;
};

std::vector<InputInfo> GetInputList(std::string inputKind)
{
	EnumInputInfo inputInfo;
	inputInfo.inputKind = inputKind;

	auto inputEnumProc = [](void *param, obs_source_t *input)
	{
		if (obs_source_get_type(input) != OBS_SOURCE_TYPE_INPUT)
			return true;

		auto inputInfo = reinterpret_cast<EnumInputInfo *>(param);

		std::string inputKind = obs_source_get_id(input);

		if (inputInfo->inputKind.empty() || inputInfo->inputKind != inputKind)
			return true;

		blog(LOG_INFO, "volume: %f", obs_mul_to_db(obs_source_get_volume(input)));
		// obs_source_set_volume(input, 0.0f);

		InputInfo value;
		value.name = obs_source_get_name(input);
		value.type = inputKind;

		inputInfo->inputs.push_back(value);
		return true;
	};
	obs_enum_sources(inputEnumProc, &inputInfo);

	return inputInfo.inputs;
}

std::vector<std::string> GetInputKindList(bool unversioned, bool includeDisabled)
{
	std::vector<std::string> ret;

	size_t idx = 0;
	const char *kind;
	const char *unversioned_kind;
	while (obs_enum_input_types2(idx++, &kind, &unversioned_kind))
	{
		uint32_t caps = obs_get_source_output_flags(kind);

		if (!includeDisabled && (caps & OBS_SOURCE_CAP_DISABLED) != 0)
			continue;

		if (unversioned)
			ret.push_back(unversioned_kind);
		else
			ret.push_back(kind);
	}
	return ret;
}

static float calculate_audio_level(const struct audio_data *data, bool muted)
{
	float audio_level = 0.0;

	if (muted)
	{
		return 0.0;
	}

	size_t nr_samples = data->frames;

	float *samples = (float *)data->data[0];
	if (!samples)
	{
		return 0.0;
	}
	float sum = 0.0;
	for (size_t i = 0; i < nr_samples; i++)
	{
		float sample = samples[i];
		sum += sample * sample;
	}
	audio_level = (float)(sqrtf(sum / nr_samples));
	return audio_level;
}

// function that keep a dynamic audio level to a desired level
static bool keep_audio_level(float audio_level, float audio_volume, float target_level, float *newAudioLevel, float min_detect_level, float margin_level)
{
	float level_change = target_level - (audio_level * audio_volume);
	// blog(LOG_INFO, "level_change: %f", level_change);
	// blog(LOG_INFO, "target_level: %f", target_level);
	// blog(LOG_INFO, "audio_level: %f", audio_level);
	// blog(LOG_INFO, "min_detect_level: %f", min_detect_level);
	// if absolute value of level change is more than smoothing level,
	// then we need to change the level
	if ((audio_level * audio_volume) < min_detect_level) {
		return false;
	}
	if (fabsf(level_change) < margin_level)
		return false;
	// Level is too low
	if (level_change > 0.0)
	{
		// *newAudioLevel = target_level - level_change;
		*newAudioLevel = CLAMP(audio_volume + 0.1, min_detect_level, 1.0f);
		return true;
	}
	// Level is too high
	else
	{
		// *newAudioLevel = target_level - level_change;
		*newAudioLevel = CLAMP(audio_volume - 0.1, min_detect_level, 1.0f);
		return true;
	}
	return false;
}

void source_audio_capture_callback(void *vptre, obs_source_t *source,
								   const struct audio_data *audio, bool muted)
{
	float inputAudioLevel = calculate_audio_level(audio, muted);

	// float inputAudioLevelInDb = obs_mul_to_db(inputAudioLevel);

	float desiredAudioLevelInDb = -20.0;
	float desiredAudioLevel = obs_db_to_mul(desiredAudioLevelInDb);

	float newAudioLevel = 0.0;
	if (keep_audio_level(inputAudioLevel, obs_source_get_volume(source), desiredAudioLevel, &newAudioLevel, 0.05, 0.01))
	{
		blog(LOG_INFO, "newAudioLevel: %f", newAudioLevel);
		obs_source_set_volume(source, newAudioLevel);
	}
	// double desiredAudioLevel = 0.05;
	// blog(LOG_INFO, "desiredAudioLevel: %f", desiredAudioLevel);
	// Always keep volume at -25dB
	// const double delta = inputAudioLevel - desiredAudioLevel;
	// blog(LOG_INFO, "delta: %f", delta);
	// obs_source_set_volume(source, obs_db_to_mul(-25.0));

	// } else {
	// 	obs_source_set_volume(source, obs_source_get_volume(source));
	// }
	// blog(LOG_INFO, "muted: %s", muted ? "true" : "false");
	// blog(LOG_INFO, "audio: %f", inputAudioLevel);
}

void source_created_signal(void *param, calldata_t *data)
{
	// std::vector<std::string> inputKinds = GetInputKindList(false, true);
	// blog(LOG_INFO, "-----------------------------------------");
	// for (auto &inputKind : inputKinds) {
	//     blog(LOG_INFO, "input kind: %s", inputKind.c_str());
	//     std::vector<InputInfo> values = GetInputList(inputKind);
	//     for (auto &value : values) {
	//         blog(LOG_INFO, "  name: %s", value.name.c_str());
	//     }
	// }
	// blog(LOG_INFO, "-----------------------------------------");
	obs_source_t *source = (obs_source_t *)calldata_ptr(data, "source");
	if (obs_source_get_type(source) != OBS_SOURCE_TYPE_INPUT)
		return;

	std::string inputKind = obs_source_get_id(source);
	if (inputKind == "coreaudio_input_capture")
	{
		blog(LOG_INFO, "input kind: %s, name : %s", inputKind.c_str(), obs_source_get_name(source));
		obs_source_add_audio_capture_callback(source, source_audio_capture_callback, nullptr);
	}
}

void source_volume_signal(void *param, calldata_t *data)
{
	float mul = (float)calldata_float(data, "volume");
	blog(LOG_INFO, "volume changed : %f", obs_mul_to_db(mul));
}

bool obs_module_load(void)
{
	blog(LOG_INFO, "plugin loaded successfully (version %s)", PLUGIN_VERSION);

	auto iso = obs_get_signal_handler();

	signal_handler_connect(iso, "source_create", source_created_signal, nullptr);
	signal_handler_connect(iso, "source_volume", source_volume_signal, nullptr);

	return true;
}

void obs_module_unload()
{
	blog(LOG_INFO, "plugin unloaded");
}
