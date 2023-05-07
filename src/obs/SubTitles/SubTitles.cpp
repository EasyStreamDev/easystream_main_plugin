#include "SubTitles.hpp"

namespace es::obs::sub_titles
{
    void run(void *)
    {
        blog(LOG_INFO, "### SubTitles - STARTING...");
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        blog(LOG_INFO, "### SubTitles - STARTED");
        while (1)
        {
            obs_source_t *text_source = obs_get_source_by_name("ES_SubTitles");

            if (!text_source)
                continue;

            std::vector<std::string> words = {
            "There are no heroes in a punk rock band.",
            "In the end, he realized he could see sound and hear words.",
            "I am happy to take your donation; any amount will be greatly appreciated.",
            "The tour bus was packed with teenage girls heading toward their next adventure.",
            "I would be delighted if the sea were full of cucumber juice.",
            "The delicious aroma from the kitchen was ruined by cigarette smoke.",
            "Having no hair made him look even hairier.",
            "He had a hidden stash underneath the floorboards in the back room of the house."};
            std::string str = words[rand() % 8];


            obs_data_t *text_settings = obs_data_create();
            obs_data_set_string(text_settings, "text", str.c_str());
            obs_source_update(text_source, text_settings);
            obs_data_release(text_settings);

            obs_source_release(text_source);
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
    }
}