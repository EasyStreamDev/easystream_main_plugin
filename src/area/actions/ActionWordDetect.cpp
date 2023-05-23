/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ActionWordDetect
*/

#include "ActionWordDetect.hpp"

namespace es::area
{
    ActionWordDetect::ActionWordDetect(Reaction *reaction, const size_t &area_id, const json &param, const std::function<std::vector<std::string>()> &func)
        : Action(reaction, area_id, param), _getTranscriptWords(func)
    {
        this->_words = param.at("words").get<std::vector<std::string>>();
    }

    ActionWordDetect::~ActionWordDetect()
    {
    }

    void ActionWordDetect::Solve()
    {
        std::vector<std::string> words = _getTranscriptWords();

        // std::vector<std::string> words = {
        //     "There are no heroes in a punk rock band.",
        //     "In the end, he realized he could see sound and hear words.",
        //     "I am happy to take your donation; any amount will be greatly appreciated.",
        //     "The tour bus was packed with teenage girls heading toward their next adventure.",
        //     "I would be delighted if the sea were full of cucumber juice.",
        //     "The delicious aroma from the kitchen was ruined by cigarette smoke.",
        //     "Having no hair made him look even hairier.",
        //     "He had a hidden stash underneath the floorboards in the back room of the house."};
        // std::string str = words[rand() % 8];

        for (const auto &w : words)
        {
            std::cerr << "Checking word: [" << w << "]" << std::endl;

            for (const auto &tF : _words)
            {
                if (w.find(tF))
                {
                    _isTrue = true;
                    return;
                }
            }
        }

        // for (const std::string word : this->_words)
        // {
        //     if (str.find(word) != std::string::npos)
        //     {
        //         this->_isTrue = true;
        //         break;
        //     }
        // }
    }

    action_t ActionWordDetect::ToStruct()
    {
        return {
            _id,
            ActionType::WORD_DETECT,
            {{"words", this->_words}}};
    }
}
