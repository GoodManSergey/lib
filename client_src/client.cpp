#include "client.h"

std::string Client::json_to_string(const Json::Value& json)
{
    Json::FastWriter fastWriter;
    std::string output = fastWriter.write(json);

    return std::move(output + "\v");
};

std::string Client::add_author_msg(const std::string& name)
{
    Json::Value root;
    Json::Value data;
    data["name"] = name;
    root["command"] = "add_author";
    root["data"] = std::move(data);

    return std::move(json_to_string(root));
};

std::string Client::get_author_msg(int id)
{
    Json::Value root;
    Json::Value data;
    data["id"] = id;
    root["command"] = "get_author_by_id";
    root["data"] = std::move(data);

    return std::move(json_to_string(root));
};

std::string Client::change_author_msg(int id, const std::string& name)
{
    Json::Value root;
    Json::Value data;
    data["id"] = id;
    data["name"] = name;
    root["command"] = "change_author";
    root["data"] = std::move(data);

    return std::move(json_to_string(root));
};

std::string Client::delete_author_msg(int id)
{
    Json::Value root;
    Json::Value data;
    data["id"] = id;
    root["command"] = "delete_author_by_id";
    root["data"] = std::move(data);

    return std::move(json_to_string(root));
};

std::string Client::add_book_msg(int author_id, const std::string& title)
{
    Json::Value root;
    Json::Value data;
    data["author_id"] = author_id;
    data["title"] = title;
    root["command"] = "add_book";
    root["data"] = std::move(data);

    return std::move(json_to_string(root));
};

std::string Client::get_book_msg(int id)
{
    Json::Value root;
    Json::Value data;
    data["id"] = id;
    root["command"] = "get_book";
    root["data"] = std::move(data);

    return std::move(json_to_string(root));
};

std::string Client::change_book_msg(int id, int author_id, const std::string& title)
{
    Json::Value root;
    Json::Value data;
    data["id"] = id;
    data["author_id"] = author_id;
    data["title"] = title;
    root["command"] = "change_book";
    root["data"] = std::move(data);

    return std::move(json_to_string(root));
};

std::string Client::delete_book_msg(int id)
{
    Json::Value root;
    Json::Value data;
    data["id"] = id;
    root["command"] = "delete_book";
    root["data"] = std::move(data);

    return std::move(json_to_string(root));
};

std::string Client::get_all_author_books_msg(int id)
{
    Json::Value root;
    Json::Value data;
    data["id"] = id;
    root["command"] = "get_all_author_books";
    root["data"] = std::move(data);

    return std::move(json_to_string(root));
};

std::string Client::get_response()
{
    std::string response;
    bool resp_gotten = false;
    while(true)
    {
        m_response_mutex.lock();
        if (m_has_response)
        {
            response = std::move(m_response);
            m_has_response = false;
            resp_gotten = true;
        }
        m_response_mutex.unlock();

        if (resp_gotten)
        {
            break;
        }
    }

    return std::move(response);
};

void Client::send_to_queue(std::string&& request)
{
    m_send_queue_mutex.lock();
    m_send_queue.push(std::move(request));
    m_send_queue_mutex.unlock();
};