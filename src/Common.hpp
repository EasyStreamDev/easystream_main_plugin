/*
** EassyStream EIP, 2021
** easystream_main_plugin
** File description:
** common
*/

#ifndef COMMON_HPP_
#define COMMON_HPP_

// Common C includes
#include <cfloat>
#include <cinttypes>
#include <climits>
#include <clocale>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <cstddef>

// Common C++ includes
#include <algorithm>
#include <array>
#include <limits>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <functional>
#include <atomic>
#include <numeric>
#include <condition_variable>
#include <list>
#include <thread>
#include <fstream>
#include <sstream>

// QT includes
#include <QStringList>
#include <QRegularExpression>
#include <QLibrary>

// Common OBS includes
#include <obs-module.h>
#include <obs.h>
#include <obs-frontend-api.h>
#include <util/platform.h>
#include <util/config-file.h>
#include <util/util_uint64.h>
#include <media-io/audio-resampler.h>

#define __FUNCTION_NAME__ __func__

// Common Files Includes
#include "obs/Memory.hpp"
#include "plugin-macros.generated.h"
#include "Macros.hpp"
#include "./nlohmann/json.hpp"

namespace es
{
    namespace area
    {
        enum class ActionType
        {
            WORD_DETECT,
            APP_LAUNCH,
            KEY_PRESSED,
        };

        enum class ReactionType
        {
            SCENE_SWITCH,
            TOGGLE_AUDIO,
        };

        typedef struct action_s
        {
            ActionType type;
            size_t id;
            nlohmann::json params;
        } action_t;

        typedef struct reaction_s
        {
            ReactionType type;
            size_t id;
            nlohmann::json params;
        } reaction_t;

        typedef struct area_s
        {
            std::string name;
            size_t id;
            bool is_active;
            action_t action_data;
            reaction_t reaction_data;
        } area_t;
    }
}

#endif /* !COMMON_HPP_ */