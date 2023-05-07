/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** ReactionStreamStop
*/

#include "ReactionStreamStop.hpp"

namespace es::area
{
    ReactionStreamStop::ReactionStreamStop(const size_t &area_id, const std::string &name, const json &param)
        : Reaction(area_id, name, param)
    {
    }

    ReactionStreamStop::~ReactionStreamStop()
    {
    }

    void ReactionStreamStop::Resolve()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(_delay * 1000));

        if (obs_frontend_streaming_active())
        {
            obs_frontend_streaming_stop();
        }

        blog(LOG_INFO, "###  - STREAMING STOPPED.");
    }

    reaction_t ReactionStreamStop::ToStruct()
    {
        return {
            _id,
            "",
            ReactionType::STOP_STREAMING,
            {}};
    }
}