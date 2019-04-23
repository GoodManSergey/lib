#include "string_queue.h"


std::string StringQueue::get()
{
    if (m_has_msg)
    {
        std::lock_guard<std::mutex> lock(m_msg_mutex);
        std::string msg = std::move(m_queue.front());
        m_queue.pop();
        return std::move(msg);
    }
    else
    {
        std::unique_lock<std::mutex> lock(m_msg_mutex);
        while(!m_has_msg)
        {
            m_cond_var.wait(lock);
        }
        if (m_queue.size() == 1)
        {
            m_has_msg = false;
        }

        std::string msg = std::move(m_queue.front());
        m_queue.pop();
        return std::move(msg);
    }
}

void StringQueue::add(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(m_msg_mutex);
    m_queue.push(std::move(msg));
    m_has_msg = true;
    m_cond_var.notify_one();
}