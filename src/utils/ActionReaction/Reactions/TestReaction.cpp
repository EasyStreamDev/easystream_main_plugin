/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** TestReaction
*/

#include "TestReaction.hpp"

es::TestReaction::TestReaction(const size_t &area_id)
    : Reaction(area_id)
{
}

es::TestReaction::~TestReaction()
{
}

void es::TestReaction::Resolve()
{
    blog(LOG_INFO, "The Reponse worked well");
}
