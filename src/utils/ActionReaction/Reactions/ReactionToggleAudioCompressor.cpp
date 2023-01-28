/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ReactionToggleAudioCompressor
*/

#include "ReactionToggleAudioCompressor.hpp"


es::ReactionToggleAudioCompressor::ReactionToggleAudioCompressor(const size_t &area_id, const std::string &name, const json &param)
    : Reaction(area_id, name, param)
{
    _toggle = param.at("toggle").get<bool>();
}

es::ReactionToggleAudioCompressor::~ReactionToggleAudioCompressor()
{
}

void es::ReactionToggleAudioCompressor::Resolve()
{
    // for (auto [audioLevelerName, audioLevelerPtr] : tracker->getAudioMap()) {
    //     audioLevelerPtr->SetActive(_toggle);
    // }
}

es::area::reaction_t es::ReactionToggleAudioCompressor::ToStruct()
{
    return {
        _id,
        "",
        es::area::ReactionType::TOGGLE_AUDIO_COMPRESSOR,
        {{"toggle", _toggle}}
    };
}