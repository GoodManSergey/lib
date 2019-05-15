#ifndef LIBRARY_STRING_QUEUE_H
#define LIBRARY_STRING_QUEUE_H

#include <string>
#include <mutex>
#include <queue>
#include <atomic>
#include <iostream>


class StringQueue
{
public:
    StringQueue() = default;

    void add(std::string&& msg);
    std::string get();

private:
    std::queue<std::string> m_queue;
    std::mutex m_msg_mutex;
};


#endif //LIBRARY_STRING_QUEUE_H
