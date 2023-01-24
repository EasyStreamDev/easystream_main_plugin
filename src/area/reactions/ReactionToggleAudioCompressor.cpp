/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ReactionToggleAudioCompressor
*/

#include "ReactionToggleAudioCompressor.hpp"

namespace es::area
{
    ReactionToggleAudioCompressor::ReactionToggleAudioCompressor(const size_t &area_id, const json &param)
        : Reaction(area_id, param)
    {
        _toggle = param.at("toggle").get<bool>();
    }

    ReactionToggleAudioCompressor::~ReactionToggleAudioCompressor()
    {
    }

    void ReactionToggleAudioCompressor::Resolve()
    {
        // for (auto [audioLevelerName, audioLevelerPtr] : tracker->getAudioMap()) {
        //     audioLevelerPtr->SetActive(_toggle);
        // }
    }

    reaction_t ReactionToggleAudioCompressor::ToStruct()
    {
        return {
            _id,
            "",
            es::area::ReactionType::TOGGLE_AUDIO_COMPRESSOR,
            {{"toggle", _toggle}}};
    }
}
