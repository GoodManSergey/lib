#include "string_queue.h"


std::string StringQueue::get()
{
    std::lock_guard<std::mutex> lock(m_msg_mutex);
    if (!m_queue.empty())
    {
        std::string msg = m_queue.front();
        m_queue.pop();
        return std::move(msg);
    }
    else
    {
        /*TODO не проще возвращать optional? Интерфейс снаружи станет более понятным для человека который это впервые видит */
        return std::move(std::string(""));
    }
}

void StringQueue::add(std::string&& msg)
{
    std::lock_guard<std::mutex> lock(m_msg_mutex);
    m_queue.push(std::move(msg));
}