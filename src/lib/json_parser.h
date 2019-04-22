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
    std::string set_empty_tmpl() override;

    std::string to_string();

    result<storage_data> get_storage(const std::string& file_str) override;

    result<std::string> add_node(const std::string& node_name, const std::string& node_next_id, std::function<int(Json::Value&)> node_filler);
    result<std::string> add_book(std::shared_ptr<const Book> book) override;
    result<std::string> add_author(std::shared_ptr<const Author> author) override;
    
    result<std::string> change_node_by_id(const std::string& node_name, int id, std::function<void(Json::Value&)> node_changer);
    result<std::string> change_book(std::shared_ptr<const Book> book) override;
    result<std::string> change_author(std::shared_ptr<const Author> author) override;

    result<std::string> delete_node_by_id(int id, const std::string& node_name);
    result<std::string> delete_book(int book_id) override;
    result<std::string> delete_author(int author_id) override;
    
    private:
    Json::Value m_root;
};
