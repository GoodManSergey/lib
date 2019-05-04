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
    while (m_work)
    {
        std::string msg = m_response.get();
        if (msg.length() != 0)
        {
            return std::move(msg);
        }
    }

    return {};
};

void Client::send_to_server(std::string&& request)
{
    m_send_queue.add(std::move(request));
};

void Client::proc_msg(std::string&& msg)
{

}

void Client::init(int server_port, const std::string &server_host)
{
    m_server->create_socket_fd();
    m_server->fill_addr(server_port);
    m_server->set_remote_addr(server_host);
    if (m_server->connect_socket() != result_code::OK)
    {
        assert(false);
    }

}

void Client::read_write()
{
    address server_addr = m_server->return_address();
    while (m_work)
    {
        std::string send_msg = m_send_queue.get();
        if (send_msg.length() != 0)
        {
            message send(send_msg, server_addr);
            auto send_res = m_server->send_msg(send);
            if (send_res != result_code::OK)
            {
                assert(false);
            }
        }

        message recv_msg = m_server->recv_msg();

        if (recv_msg.m_data.length() == 0)
        {
            continue;
        }
        else
        {
            std::string msg;
            for (char& c : recv_msg.m_data)
            {
                std::cout<<c;
                if (c == '\v')
                {
                    proc_msg(std::move(msg));
                    msg = "";
                }
                msg += c;
            }
        }
    }
}