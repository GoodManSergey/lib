#include "string_queue.h"


std::string StringQueue::get()
{
    if (m_has_msg)
    {
        std::lock_guard<std::mutex> lock(m_msg_mutex);
        if (m_queue.size() == 1)
        {
            m_has_msg = false;
        }
        std::string msg = std::move(m_queue.front());
        m_queue.pop();
        return std::move(msg);
    }
    else
    {
        return std::move(std::string(""));
    }
}

void StringQueue::add(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(m_msg_mutex);
    m_queue.push(std::move(msg));
    m_has_msg = true;
}