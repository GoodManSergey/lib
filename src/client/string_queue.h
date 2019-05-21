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

    /* TODO Стилистическое замечание, в данном случае запись 'const std::string& msg' ничем не будет по существу отличаться,
     * но будет являтся более идиоматичной и будет привлекать к себе меньше внимания, здесь ничего страшного, но впредь в наших проектах лучше использовать её
     */
    void add(std::string&& msg);
    std::string get();

private:
    std::queue<std::string> m_queue;
    std::mutex m_msg_mutex;
};


#endif //LIBRARY_STRING_QUEUE_H
