/*
** Easystream EIP, 2021
** easystream_main_plugin
** File description:
** json
*/

#ifndef JSON_HPP_
#define JSON_HPP_

#include <obs.hpp>
#include "../utils/nlohmann/json.hpp"

using json = nlohmann::json;

namespace es {
    namespace utils {
        namespace Json {
            bool JsonArrayIsValidObsArray(json j);
            obs_data_t *JsonToObsData(json j);
            json ObsDataToJson(obs_data_t *d, bool includeDefault = false);
        }
    }
}

#endif /* !JSON_HPP_ */
