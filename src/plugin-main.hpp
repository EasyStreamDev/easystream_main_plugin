/*
** Easystream EIP, 2021
** easystream_main_plugin
** File description:
** utils
*/

#ifndef MAIN_HPP_
#define MAIN_HPP_
#include "Common.hpp"
#include <iostream>
#include <chrono>

#include "obs/SourceTracker.hpp"
#include "utils/Thread.hpp"
#include "utils/ActionReaction/ActionReactionMain.hpp"
// #include "obs/speechRecognition/transcription/TranscriptorManager.hpp"

os_cpu_usage_info_t *GetCpuUsageInfo();

#endif /* !MAIN_HPP_ */
