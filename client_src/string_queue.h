#ifndef LIBRARY_STRING_QUEUE_H
#define LIBRARY_STRING_QUEUE_H

#include <string>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>


class StringQueue
{
public:
    void add(const std::string& msg);
    std::string get();

private:
    std::queue<std::string> m_queue;
    std::mutex m_msg_mutex;
    std::condition_variable  m_cond_var;
    std::atomic<bool> m_has_msg;
};


#endif //LIBRARY_STRING_QUEUE_H
