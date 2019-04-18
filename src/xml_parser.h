#pragma once
#include<string>
#include"storage_data.h"
#include<pugixml.hpp>
#include"result.h"
#include"parser.h"
#include<iostream>
#include<functional>
#include<algorithm>
#include<sstream>
#include<memory>
#include<vector>

class XmlParser: public Parser
{
    public:
    std::string set_empty_tmpl() override;

    std::string to_string();

    result<storage_data> get_storage(const std::string& file_str) override;

    result<std::string> add_node(const std::string& nodes_path,
                                 const std::string& next_id_path, std::function<void(pugi::xml_node)> node_filler);

    result<std::string> add_book(std::shared_ptr<const Book> book) override;

    result<std::string> add_author(std::shared_ptr<const Author> author) override;

    result<std::string> change_node(const std::string& nodes_path,
                                     std::function<void(pugi::xml_node)> node_filler, std::function<bool(pugi::xml_node)> node_finder);

    result<std::string> change_book(std::shared_ptr<const Book> book) override;

    result<std::string> change_author(std::shared_ptr<const Author> author) override;

    result<std::string> delete_node_by_id(const std::string& nodes_path, int node_id);

    result<std::string> delete_book(int book_id) override;
    result<std::string> delete_author(int author_id) override;
    
    ~XmlParser() override = default;

    private:
        pugi::xml_document m_doc;
};
