/*
** EPITECH PROJECT, 2023
** easystream_main_plugin
** File description:
** ReactionRecordingStop
*/

#include "ReactionRecordingStop.hpp"

es::area::ReactionRecordingStop::ReactionRecordingStop(const size_t &area_id, const std::string &name, const json &param)
    : es::area::Reaction(area_id, name, param)
{
}

es::area::ReactionRecordingStop::~ReactionRecordingStop()
{
}

void es::area::ReactionRecordingStop::Resolve()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(_delay * 1000));

    obs_frontend_recording_stop();

    blog(LOG_INFO, "###  - RECORDING STOPPED.");
}

es::area::reaction_t es::area::ReactionRecordingStop::ToStruct()
{
    return {
        _id,
        "",
        es::area::ReactionType::STOP_RECORDING,
        {}};
}