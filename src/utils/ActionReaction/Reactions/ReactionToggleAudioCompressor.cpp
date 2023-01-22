/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ReactionToggleAudioCompressor
*/

#include "ReactionToggleAudioCompressor.hpp"
#include "../../../obs/SourceTracker.hpp"


es::ReactionToggleAudioCompressor::ReactionToggleAudioCompressor(const size_t &area_id, const json &param)
    : Reaction(area_id, param)
{
    // _toggle = param["toggle"].get<bool>();
}

es::ReactionToggleAudioCompressor::~ReactionToggleAudioCompressor()
{
}

void es::ReactionToggleAudioCompressor::Resolve()
{
    std::unordered_map<std::string, std::shared_ptr<es::obs::AutoAudioLeveler>> audioLevelers = es::obs::SourceTracker::getAudioMap();

    for (auto [audioLevelerName, audioLevelerPtr] : audioLevelers) {
        audioLevelerPtr->SetActive(_toggle);
    }
}
