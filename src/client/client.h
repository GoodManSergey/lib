#ifndef LIBRARY_CLIENT_H
#define LIBRARY_CLIENT_H

#include <string>
#include <mutex>
#include <queue>
#include "jsoncpp/json/json.h"
#include "string_queue.h"
#include <atomic>


class Client
{
public:
    Client()
    {}

    virtual ~Client() = default;
    void init(int server_port, const std::string& server_host);
    virtual void read_write() = 0;

    std::string json_to_string(const Json::Value& json);

    std::string add_author_msg(const std::string& name);

    std::string get_author_msg(int id);

    std::string change_author_msg(int id, const std::string& name);

    std::string delete_author_msg(int id);

    std::string add_book_msg(int author_id, const std::string& title);

    std::string get_book_msg(int id);

    std::string change_book_msg(int id, int author_id, const std::string& title);

    std::string delete_book_msg(int id);

    std::string get_all_author_books_msg(int id);

    std::string get_response();

    void send_to_queue(std::string&& request);

protected:
    StringQueue m_send_queue;
    StringQueue m_response;
    std::atomic<bool> m_work{true};

};

#endif //LIBRARY_CLIENT_H
