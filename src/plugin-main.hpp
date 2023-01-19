/*
** Easystream EIP, 2021
** easystream_main_plugin
** File description:
** utils
*/

#ifndef MAIN_HPP_
#define MAIN_HPP_
#include "Common.hpp"

#include "obs/SourceTracker.hpp"
#include "utils/Thread.hpp"
#include "utils/ActionResponse/ActionResponseMain.hpp"
#include "utils/ActionResponse/Actions/TestAction.hpp"
#include "utils/ActionResponse/Responses/TestResponses.hpp"

os_cpu_usage_info_t *GetCpuUsageInfo();

#endif /* !MAIN_HPP_ */
