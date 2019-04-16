#pragma once
#include<jsoncpp/json/json.h>
#include<memory>
#include"author.h"
#include"book.h"
#include<vector>
#include"result.h"
#include"parser.h"
#include<string>
#include"storage_data.h"
#include<iostream>
#include<algorithm>


class JsonParser: public Parser
{
    public:
    std::string set_empty_tmpl();

    std::string to_string();

    result<storage_data> get_storage(const std::string& file_str);

    result<std::string> add_node(const std::string& file_str, const std::string& node_name, const std::string node_next_id, std::function<int(Json::Value&)> node_filler);
    result<std::string> add_book(const std::string& file_str, std::shared_ptr<const Book> book);
    result<std::string> add_author(const std::string& file_str, std::shared_ptr<const Author> author);
    
    result<std::string> change_node_by_id(const std::string& file_str, const std::string& node_name, int id, std::function<void(Json::Value&)> node_changer);
    result<std::string> change_book(const std::string& file_str, std::shared_ptr<const Book> book);
    result<std::string> change_author(const std::string& file_str, std::shared_ptr<const Author> author);

    result<std::string> delete_node_by_id(const std::string& file_str, int id, const std::string& node_name);
    result<std::string> delete_book(const std::string& file_str, int book_id);
    result<std::string> delete_author(const std::string& file_str, int author_id);
    
    private:
    Json::Value m_root;
};
