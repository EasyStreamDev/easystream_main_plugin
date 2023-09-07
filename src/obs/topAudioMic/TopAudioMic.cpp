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
    double elapsedTime = (std::clock() - topAudioMic->_sourcesVolumes[obs_source_get_name(source)].StartTime) / (double)CLOCKS_PER_SEC;

    topAudioMic->_sourcesVolumes[obs_source_get_name(source)].Sum += inputAudioLevel;
    topAudioMic->_sourcesVolumes[obs_source_get_name(source)].Number += 1;

    topAudioMic->UpdateTopMic();

    blog(LOG_INFO, "[es::Obs::topAudioMic] input: %s, have an audio input of: %f", obs_source_get_name(source), inputAudioLevel);
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

void es::obs::TopAudioMic::AddNewAudioSource(obs_source_t *source)
{
    std::string name = obs_source_get_name(source);
    if (!name.rfind("Audio", 0) == 0)
        return;
    VolumesData Data;
    Data.StartTime = std::clock();
    _sourcesVolumes[name] = Data;
    
    obs_source_add_audio_capture_callback(source, InputAudioCaptureCallback, this);

    blog(LOG_INFO, "[es::Obs::topAudioMic] new input added to check top Mic: %s", obs_source_get_name(source));
}

void es::obs::TopAudioMic::AddNewVideoSource(obs_source_t *source)
{
    _sourcesVideo.push_back(obs_source_get_name(source));

    blog(LOG_INFO, "[es::Obs::topAudioMic] new input added to check top Mic: %s", obs_source_get_name(source));
}

void es::obs::TopAudioMic::UpdateTopMic()
{
    float TopVolumeMean = 0;
    std::string TopAudioSource;

    for (auto [name, data] : _sourcesVolumes)
    {
        float ActualSum = data.Sum;
        if (data.Number > 0)
            ActualSum /= data.Number;
        if (ActualSum > TopVolumeMean)
        {
            TopVolumeMean = ActualSum;
            TopAudioSource = name;
            blog(LOG_INFO, "[es::Obs::topAudioMic] top Mic is: %s", TopAudioSource.c_str());
        }
    }

    for (auto name : _sourcesVideo)
    {
        obs_source_set_enabled(obs_get_source_by_name(name.c_str()), false);
    }
    std::string name = "Video";
    name += TopAudioSource.back();
    if (std::find(_sourcesVideo.begin(), _sourcesVideo.end(), name) != _sourcesVideo.end())
    {
        obs_source_set_enabled(obs_get_source_by_name(name.c_str()), true);
    }
}