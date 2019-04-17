#pragma once
#include"result.h"
#include"storage.h"
#include"author.h"
#include"book.h"
#include<assert.h>
#include<iostream>
#include<memory>
#include<unordered_map>
#include<vector>
#include<algorithm>


class Library
{
    public:
    Library(std::unique_ptr<Storage> storage);

    result<int> add_author(std::unique_ptr<Author> author);

    result<std::shared_ptr<const Author>> get_author_by_id(int author_id);

    result<std::vector<std::shared_ptr<const Book>>> get_authors_books(int author_id);
    
    result<int> add_book(std::unique_ptr<Book> book);
    
    result<std::shared_ptr<const Book>> get_book_by_id(int book_id);

    result<int> change_author(std::unique_ptr<Author> author);

    result<int> change_book(std::unique_ptr<Book> book);
    
    result_code delete_author(int author_id);

    result_code delete_book(int book_id);

    private:
    int m_next_author_id;
    int m_next_book_id;
    std::unordered_map<int, std::shared_ptr<Author>> m_author_list;
    std::unordered_map<int, std::shared_ptr<Book>> m_book_list;
    std::unordered_map<int, std::vector<int>> m_authors_books;
    std::unique_ptr<Storage> pm_storage;

};
