/*
** Easystream EIP, 2021
** easystream_main_plugin
** File description:
** eventsTransitions
*/

#include "../SourceTracker.hpp"

void es::obs::SourceTracker::handleTransitionCreated(obs_source_t *source)
{
	std::string transitionName(obs_source_get_name(source));
	std::string transitionKind(obs_source_get_id(source));
	bool transitionFixed = obs_transition_fixed(source);
	// blog(LOG_INFO, "### [SourceTracker::handleTransitionCreated]: %s %s", transitionName.c_str(), transitionKind.c_str());
}

void es::obs::SourceTracker::handleTransitionRemoved(obs_source_t *source)
{
	std::string name(obs_source_get_name(source));
	// blog(LOG_INFO, "### [SourceTracker::handleTransitionRemoved]: %s", name.c_str());
}

void es::obs::SourceTracker::handleTransitionNameChanged(obs_source_t *, std::string oldTransitionName, std::string transitionName)
{
	// blog(LOG_INFO, "### [SourceTracker::handleTransitionNameChanged]: %s -> %s", oldTransitionName.c_str(), transitionName.c_str());
}