/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ReactionToggleAudioCompressor
*/

#include "ReactionToggleAudioCompressor.hpp"
#include "../../PluginManager.hpp"

namespace es::area
{
    ReactionToggleAudioCompressor::ReactionToggleAudioCompressor(const size_t &area_id, const std::string &name, const json &param)
        : Reaction(area_id, name, param)
    {
        try
        {
            _toggle = param.at("audio-source").get<std::string>() == "false" ? false : true;
        }
        catch (std::exception e)
        {
            _toggle = true;
        }
    }

    ReactionToggleAudioCompressor::~ReactionToggleAudioCompressor()
    {
    }

    void ReactionToggleAudioCompressor::Resolve()
    {
        for (auto [audioLevelerName, audioLevelerPtr] : this->_pm->GetSourceTracker()->getAudioMap())
        {
            audioLevelerPtr->SetActive(_toggle);
        }
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
