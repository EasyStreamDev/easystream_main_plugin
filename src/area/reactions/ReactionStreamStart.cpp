/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** ReactionStreamStart
*/

#include "ReactionStreamStart.hpp"

namespace es::area
{
    ReactionStreamStart::ReactionStreamStart(const size_t &area_id, const std::string &name, const json &param)
        : Reaction(area_id, name, param)
    {
    }

    ReactionStreamStart::~ReactionStreamStart()
    {
    }

    void ReactionStreamStart::Resolve()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(_delay * 1000));

        if (!obs_frontend_streaming_active())
        {
            obs_frontend_streaming_start();
        }

        blog(LOG_INFO, "###  - STREAMING STARTED.");
    }

    reaction_t ReactionStreamStart::ToStruct()
    {
        return {
            _id,
            "",
            ReactionType::START_STREAMING,
            {}};
    }
}
