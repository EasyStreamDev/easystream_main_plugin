/*
** Thread safe queue
** File description:
** TSQueue
*/

#include "../include/TSRequestQueue.hpp"

namespace es::server
{
    void TSRequestQueue::ts_push(const std::pair<Shared<AsioTcpConnection>, json> &item)
    {
        std::scoped_lock<std::mutex> lock(this->m_Mutex);

        m_Queue.push(item);
    }

    void TSRequestQueue::ts_push(Shared<AsioTcpConnection> socket, const std::vector<json> &requests)
    {
        std::scoped_lock<std::mutex> lock(this->m_Mutex);

        for (const auto &r : requests)
        {
            m_Queue.push(std::make_pair(socket, r));
        }
    }

    void TSRequestQueue::ts_pop(void)
    {
        std::scoped_lock<std::mutex> lock(this->m_Mutex);

        m_Queue.pop();
    }

    bool TSRequestQueue::empty(void) const
    {
        return m_Queue.empty();
    }

    const std::pair<Shared<AsioTcpConnection>, json> &TSRequestQueue::front(void) const
    {
        return m_Queue.front();
    }
}