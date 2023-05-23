#ifndef SUBTITLES_HPP_
#define SUBTITLES_HPP_

#include "../../Common.hpp"
#include "../../utils/Utils.hpp"

namespace es{
    class PluginManager;
}

namespace es::obs::sub_titles
{
    void run(es::PluginManager *PluginManager);
}

#endif /* !SUBTITLES_HPP_ */