/*
** Thread safe queue
** File description:
** TSQueue
*/

#ifndef TSQUEUE_HPP_
#define TSQUEUE_HPP_

#include <queue>
#include <mutex>

namespace es
{
    template <typename T>
    class ThreadSafeQueue : public std::queue<T>
    {
    public:
        void ts_push(const T &item)
        {
            std::scoped_lock(m_Mutex);

            this->push(item);
        }

        void ts_push(T &&item)
        {
            std::scoped_lock(m_Mutex);

            this->push(item);
        }

        void ts_push(const std::vector<T> &items)
        {
            std::scoped_lock(m_Mutex);

            for (auto &item : items)
            {
                this->push(item);
            }
        }

        void ts_pop(void)
        {
            std::scoped_lock(m_Mutex);

            this->pop();
        }

    private:
        std::mutex m_Mutex;
    };
}

#endif /* TSQUEUE_HPP_ */