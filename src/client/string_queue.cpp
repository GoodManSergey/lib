#include "string_queue.h"

/*
 * TODO m_has_msg меняется каждый раз только в критической секции проще уже и его чтение запихнуть в критическую секцию,
 * создай метод empty(), кроме того, этот флаг вообще не сильно и нужен, проще проверять пустая ли очередь вызывая у неё метод empty()
 * каждый раз когда это необходимо.
 */

std::string StringQueue::get()
{
    if (m_has_msg)
    {
        std::lock_guard<std::mutex> lock(m_msg_mutex);
        if (m_queue.size() == 1)
        {
            m_has_msg = false;
        }
        std::string msg = m_queue.front();
        m_queue.pop();
        return std::move(msg);
    }
    else
    {
        return std::move(std::string(""));
    }
}

void StringQueue::add(std::string&& msg)
{
    std::lock_guard<std::mutex> lock(m_msg_mutex);
    std::cout<<msg;
    m_queue.push(std::move(msg));
    std::cout<<m_queue.size();
    m_has_msg = true;
}