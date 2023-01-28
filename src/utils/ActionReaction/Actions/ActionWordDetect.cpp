/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** ActionWordDetect
*/

#include "ActionWordDetect.hpp"

es::ActionWordDetect::ActionWordDetect(Reaction *reaction, const size_t &area_id, const json &param)
    : Action(reaction, area_id, param)
{
    // @todo : save string vector instead of one word
    std::vector<std::string> words = param["words"];
    this->_word = words.back();
}

es::ActionWordDetect::~ActionWordDetect()
{
}

void es::ActionWordDetect::Solve()
{
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

    if (str.find(_word) != std::string::npos)
    {
        this->_isTrue = true;
    }
}

es::area::action_t es::ActionWordDetect::ToStruct()
{
    // @todo : refer to @todo in ctor
    return {
        _id,
        es::area::ActionType::WORD_DETECT,
        {{"words", std::vector<std::string>({_word})}}};
}