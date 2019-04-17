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
    std::string set_empty_tmpl();

    std::string to_string();

    result<storage_data> get_storage(const std::string& file_str);

    result<std::string> add_node(const std::string& file_str, const std::string& nodes_path,
                                 const std::string& next_id_path, std::function<void(pugi::xml_node)> node_filler);

    result<std::string> add_book(const std::string& file_str, std::shared_ptr<const Book> book);

    result<std::string> add_author(const std::string& file_str, std::shared_ptr<const Author> author);

    result<std::string> change_node(const std::string& file_str, const std::string& nodes_path,
                                     std::function<void(pugi::xml_node)> node_filler, std::function<bool(pugi::xml_node)> node_finder);

    result<std::string> change_book(const std::string& file_str, std::shared_ptr<const Book> book);

    result<std::string> change_author(const std::string& file_str, std::shared_ptr<const Author> author);

    result<std::string> delete_node_by_id(const std::string& file_str, const std::string& nodes_path, int node_id);

    result<std::string> delete_book(const std::string& file_str, int book_id);
    result<std::string> delete_author(const std::string& file_str, int author_id);
    
    ~XmlParser() = default;

    private:
        pugi::xml_document m_doc;
};
