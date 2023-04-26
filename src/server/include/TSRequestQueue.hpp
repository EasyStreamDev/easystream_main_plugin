/*
** Thread safe queue
** File description:
** TSQueue
*/

#ifndef TSQUEUE_HPP_
#define TSQUEUE_HPP_

#include "AsioTcpConnection.hpp"
#include "common_using.hpp"
#include "../../utils/Json.hpp"

#include <queue>
#include <mutex>

namespace es::server
{
    /* Thread-safe Request Queue */
    class TSRequestQueue
    {
    public:
        void ts_push(const std::pair<Shared<AsioTcpConnection>, json> &);
        void ts_push(Shared<AsioTcpConnection>, const std::vector<json> &);
        void ts_pop(void);
        [[nodiscard]] bool empty(void) const;
        const std::pair<Shared<AsioTcpConnection>, json> &front(void) const;

    private:
        std::queue<std::pair<Shared<AsioTcpConnection>, json>> m_Queue;
        std::mutex m_Mutex;
    };
}

#endif /* TSQUEUE_HPP_ */