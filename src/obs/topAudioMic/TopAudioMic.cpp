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
    double elapsedTime = (std::clock() - topAudioMic->_sourcesVolumes[source].StartTime) / (double)CLOCKS_PER_SEC;

    topAudioMic->_sourcesVolumes[source].InputVolumes.push_back(std::make_pair(elapsedTime, inputAudioLevel));

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
    VolumesData Data;
    Data.StartTime = std::clock();
    _sourcesVolumes[source] = Data;
    
    obs_source_add_audio_capture_callback(source, InputAudioCaptureCallback, this);

    blog(LOG_INFO, "[es::Obs::topAudioMic] new input added to check top Mic: %s", obs_source_get_name(source));
}

void es::obs::TopAudioMic::AddNewVideoSource(obs_source_t *source)
{
    _sourcesVideo.push_back(source);
}

void es::obs::TopAudioMic::UpdateTopMic()
{
    obs_source_t *topSource = _actualTopSource;
    int topSourceID = 0;
    float TopVolumeMean = 0;

    int i = 0;  
    for (auto sourceVolumes : _sourcesVolumes)
    {
        float LastTime = sourceVolumes.second.InputVolumes.back().first;
        float sum = 0;
        int valueNum = 0;
        for (auto [elapsedTime, volume] : sourceVolumes.second.InputVolumes)
        {
            if (LastTime - elapsedTime < 3)
            {
                sum += volume;
                valueNum += 1;
            }
        }
        if (valueNum > 1)
            sum /= valueNum;
        if (sum > TopVolumeMean)
        {
            TopVolumeMean = sum;
            _actualTopSource = sourceVolumes.first;
            topSourceID = i;
        }
        i++;
    }

    if (_actualTopSource == topSource)
        return;
    for (auto source : _sourcesVideo)
    {
        obs_source_set_enabled(source, false);
    }
    if (_sourcesVideo[topSourceID])
        obs_source_set_enabled(_sourcesVideo[topSourceID], true);
}