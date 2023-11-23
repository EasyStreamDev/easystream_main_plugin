/*
** EPITECH PROJECT, 2021
** easystream_main_plugin
** File description:
** topAudioMic
*/

#include "TopAudioMic.hpp"

es::obs::TopAudioMic::TopAudioMic()
{
}

es::obs::TopAudioMic::~TopAudioMic()
{
}

void es::obs::TopAudioMic::InputAudioCaptureCallback(void *priv_data, obs_source_t *source, const struct audio_data *data, bool muted)
{
    if (!data || !data->frames || !priv_data)
    {
        return;
    }

    TopAudioMic *topAudioMic = static_cast<es::obs::TopAudioMic *>(priv_data);

    if (!topAudioMic || !obs_source_active(source))
    {
        return;
    }

    float inputAudioLevel = topAudioMic->CalculateAudioLevel(data, false);
    // blog(LOG_INFO, "[es::Obs::topAudioMic] input: %s, has an audio input of: %f", obs_source_get_name(source), inputAudioLevel);
    for (auto it : topAudioMic->m_MicsVolumes)
    {
        if (it.first == obs_source_get_uuid(source))
        {
            it.second.push_back({std::clock() / (double)CLOCKS_PER_SEC, inputAudioLevel});
        }
    }
    topAudioMic->UpdateTopMic();
}

float es::obs::TopAudioMic::CalculateAudioLevel(const struct audio_data *data, bool muted)
{
    float audio_level = 0.0;

    if (muted)
    {
        return 0.0;
    }

    size_t nr_samples = data->frames;

    float *samples = (float *)data->data[0];
    if (!samples)
    {
        return 0.0;
    }
    float sum = 0.0;
    for (size_t i = 0; i < nr_samples; i++)
    {
        float sample = samples[i];
        sum += sample * sample;
    }
    audio_level = (float)(sqrtf(sum / nr_samples));
    return audio_level;
}

void es::obs::TopAudioMic::AddMicDisplayLinks(MicDisplayLink Link)
{
    OBSSourceAutoRelease mic_source = obs_get_source_by_uuid(Link.MicUuid.c_str());

    // Mic source not found
    if (mic_source == nullptr)
    {
        throw es::server::RequestError("Microphone not found");
    }

    // Mic source not registered: creating callback on it
    if (m_MicDisplayLinks.find(Link.MicUuid) == m_MicDisplayLinks.end())
    {
        obs_source_add_audio_capture_callback(mic_source, InputAudioCaptureCallback, this);
        m_MicDisplayLinks.insert({Link.MicUuid, Link.DisplaysUuid});
        m_MicsVolumes.push_back(std::pair<std::string, std::vector<VolumesData>>(Link.MicUuid, {}));
        blog(LOG_INFO, "[es::Obs::topAudioMic] new MicDisplayLink added: %s", obs_source_get_name(mic_source));
    }
    else
    {
        throw es::server::RequestError("Microphone already exists in this context.");
    }
}

void es::obs::TopAudioMic::RemoveMicDisplayLinks(std::string MicUuid)
{
    OBSSourceAutoRelease mic_source = obs_get_source_by_uuid(MicUuid.c_str());

    if (mic_source.Get() == nullptr)
    {
        throw es::server::RequestError("Microphone not found");
    }

    obs_source_remove_audio_capture_callback(mic_source, InputAudioCaptureCallback, this);

    m_MicDisplayLinks.erase(MicUuid);
    m_MicsVolumes.erase(
        std::remove_if(
            m_MicsVolumes.begin(), m_MicsVolumes.end(),
            [=](std::pair<std::string, std::vector<VolumesData>> data)
            { return data.first == MicUuid; }),
        m_MicsVolumes.end());
}

void es::obs::TopAudioMic::UpdateMicDisplayLinks(std::string MicUuid, std::vector<std::string> NewDisplaysUuid)
{
    auto it = m_MicDisplayLinks.find(MicUuid);

    if (it != m_MicDisplayLinks.end())
    {
        it->second = NewDisplaysUuid;
    }
}

void es::obs::TopAudioMic::UpdateTopMic()
{
    float ActualTime = std::clock() / (double)CLOCKS_PER_SEC;

    float TopVolumeMean = 0;
    std::string TopAudioSourceUuid;

    for (auto it : m_MicsVolumes)
    {
        it.second.erase(
            std::remove_if(
                it.second.begin(), it.second.end(),
                [=](const VolumesData &Vdata)
                { return ActualTime - Vdata.Time > 2; }),
            it.second.end());

        float ActualMean = 0;
        for (auto [time, volume] : it.second)
        {
            ActualMean += volume;
        }
        if (it.second.size() > 0)
        {
            ActualMean /= it.second.size();
        }
        if (ActualMean > TopVolumeMean)
        {
            TopVolumeMean = ActualMean;
            TopAudioSourceUuid = it.first;
            blog(LOG_INFO, "[es::Obs::topAudioMic] top Mic is: %s", TopAudioSourceUuid.c_str());
        }
    }

    for (auto it : m_MicDisplayLinks)
    {
        for (std::string DisplayUuid : it.second)
        {
            obs_source_set_enabled(OBSSourceAutoRelease(obs_get_source_by_uuid(DisplayUuid.c_str())), false);
        }
    }
    for (auto it : m_MicDisplayLinks)
    {
        if (it.first == TopAudioSourceUuid)
        {
            for (std::string DisplayUuid : it.second)
            {
                obs_source_set_enabled(OBSSourceAutoRelease(obs_get_source_by_uuid(DisplayUuid.c_str())), true);
            }
        }
    }
}