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
#include <mutex>
#include <iostream>
#include <chrono>

// QT includes
#include <QStringList>
#include <QRegularExpression>
#include <QLibrary>

// Common OBS includes
#include <obs.h>
#include <obs-module.h>
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
#include "utils/Json.hpp"

namespace es
{
    namespace area
    {
        static size_t ID_COUNTER = 1; // 0 will be used as non-existing area ID

        enum class ActionType
        {
            WORD_DETECT,
            APP_LAUNCH,
            KEY_PRESSED,
        };

        static const std::string ActionTypeToString(const ActionType &t_)
        {
            switch (t_)
            {
            case ActionType::WORD_DETECT:
                return "WORD_DETECT";
            case ActionType::APP_LAUNCH:
                return "APP_LAUNCH";
            case ActionType::KEY_PRESSED:
                return "KEY_PRESSED";
            default:
                break;
            }
            return "UNKNOWN_TYPE";
        }

        enum class ReactionType
        {
            SCENE_SWITCH,
            TOGGLE_AUDIO_COMPRESSOR,
        };

        static const std::string ReactionTypeToString(const ReactionType &t_)
        {
            switch (t_)
            {
            case ReactionType::SCENE_SWITCH:
                return "SCENE_SWITCH";
            case ReactionType::TOGGLE_AUDIO_COMPRESSOR:
                return "TOGGLE_AUDIO_COMPRESSOR";
            default:
                break;
            }
            return "UNKNOWN_TYPE";
        }

        /**************/
        /* STRUCTURES */
        /**************/

        typedef struct action_s
        {
            size_t id;
            ActionType type;
            json params;
        } action_t;

        typedef struct reaction_s
        {
            size_t id;
            std::string name;
            ReactionType type;
            json params;
        } reaction_t;

        typedef struct area_s
        {
            // std::string name;
            size_t id;
            bool is_active;
            action_t action_data;
            reaction_t reaction_data;
        } area_t;
    }
}

#endif /* !COMMON_HPP_ */