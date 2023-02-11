#ifndef ES_RUNNABLE_HPP
#define ES_RUNNABLE_HPP

#include <memory>
#include <chrono>
#include <thread>

namespace es
{
    class Runnable
    {
    public:
        virtual void run(void *) = 0;

    protected:
        inline void thread_sleep_ms(unsigned int ms)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }
    };
}

#endif /* !ES_RUNNABLE_HPP */