#ifndef LIBRARY_CLIENT_H
#define LIBRARY_CLIENT_H

#include <string>
#include <mutex>
#include <queue>
#include "jsoncpp/json/json.h"

class Client
{
public:
    Client():
            m_has_response(false)
    {}

    virtual ~Client() = default;
    virtual void init(int server_port, const std::string& server_host) = 0;
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
    std::queue<std::string> m_send_queue;
    std::string m_response;
    bool m_has_response;
    std::mutex m_send_queue_mutex;
    std::mutex m_response_mutex;

};

#endif //LIBRARY_CLIENT_H
