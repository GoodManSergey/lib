#pragma once
#include<memory>
#include<string>
#include<iostream>
#include<jsoncpp/json/json.h>
#include"server_command.h"
#include"server_status.h"
#include"../lib/book.h"
#include"../lib/author.h"
#include"../lib/library.h"
#include<vector>
#include<netinet/in.h>
#include<sys/socket.h>
#include<atomic>
#include<unistd.h>
#include "../lib/socket.h"


class Server
{
    public:
    explicit Server(std::unique_ptr<Library> lib, std::unique_ptr<Socket> server_socket);

    virtual ~Server() = default;

    void init_socket(int port);

	std::string json_to_string(const Json::Value& json);

	std::string add_author(const Json::Value& data);

	std::string get_author(const Json::Value& data);

	std::string change_author(const Json::Value& data);

	std::string delete_author(const Json::Value& data);

	std::string add_book(const Json::Value& data);

	std::string get_book(const Json::Value& data);

	std::string change_book(const Json::Value& data);

	std::string delete_book(const Json::Value& data);

	std::string get_all_author_books(const Json::Value& data);

	std::string make_resp(Json::Value& data, const std::string& status);

	std::string error_return(const std::string& error);

    std::string proc_msg(const std::string& msg);

    virtual void run() = 0;

    void stop();

    protected:
    std::atomic<bool> m_work;
    std::unique_ptr<Library> pm_lib;
    std::unique_ptr<Socket> pm_server_socket;
    std::unordered_map<std::string, server_command> m_commands;
};
