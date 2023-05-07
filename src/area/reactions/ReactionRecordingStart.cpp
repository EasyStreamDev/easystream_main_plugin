/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** ReactionRecordingStart
*/

#include "ReactionRecordingStart.hpp"

es::area::ReactionRecordingStart::ReactionRecordingStart(const size_t &area_id, const std::string &name, const json &param)
    : Reaction(area_id, name, param)
{
}

es::area::ReactionRecordingStart::~ReactionRecordingStart()
{
}

void es::area::ReactionRecordingStart::Resolve()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(_delay * 1000));

    if (!obs_frontend_recording_active())
    {
        obs_frontend_recording_start();
    }

    blog(LOG_INFO, "###  - RECORDING STARTED.");
}

es::area::reaction_t es::area::ReactionRecordingStart::ToStruct()
{
    return {
        _id,
        "",
        es::area::ReactionType::START_RECORDING,
        {}};
}