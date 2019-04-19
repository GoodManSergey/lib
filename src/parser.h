#pragma once
#include"book.h"
#include"author.h"
#include"string.h"
#include"result.h"
#include"memory"
#include"storage_data.h"

class Parser
{
    public:
    virtual std::string set_empty_tmpl() = 0;
    virtual result<storage_data> get_storage(const std::string& file_str) = 0;
    virtual result<std::string> add_book(std::shared_ptr<const Book> book) = 0;
    virtual result<std::string> add_author(std::shared_ptr<const Author> author) = 0;
    virtual result<std::string> change_book(std::shared_ptr<const Book> book) = 0;
    virtual result<std::string> change_author(std::shared_ptr<const Author> author) = 0;
    virtual result<std::string> delete_book(int book_id) = 0;
    virtual result<std::string> delete_author(int author_id) = 0;
    virtual ~Parser() = default;
};
