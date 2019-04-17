#pragma once
#include<string>
#include<sstream>
#include<fstream>
#include"storage_data.h"
#include"storage.h"
#include"parser.h"
#include<iostream>
#include<memory>
#include<assert.h>
#include"result.h"
#include"result_code.h"
#include"author.h"
#include"book.h"


class FileStorage: public Storage
{
   public:
    FileStorage(std::unique_ptr<Parser> parser, std::string storage_path);

    result<std::string> get_string_from_file();

    result_code make_file(const std::string& str_data, const std::string&);

    result_code set_data_to_file(const std::string& str_data);

    storage_data make_tmpl_file();

    storage_data get_storage();

    result_code store(std::function<result<std::string>(std::string&)> parser);

    result_code add_book(std::shared_ptr<const Book> book);

    result_code add_author(std::shared_ptr<const Author> author);

    result_code change_book(std::shared_ptr<const Book> book);
    
    result_code change_author(std::shared_ptr<const Author> author);
    
    result_code delete_book(int book_id);
    
    result_code delete_author(int author_id);
    
private:
    std::unique_ptr<Parser> pm_parser;
    std::string m_storage_path;
};
