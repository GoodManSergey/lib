#pragma once
#include"author.h"
#include"book.h"
#include<vector>
#include<memory>

struct storage_data
{
    storage_data(int next_book_id, int next_author_id, std::vector<std::shared_ptr<Author>> author_list, std::vector<std::shared_ptr<Book>> book_list):
        m_next_book_id(next_book_id),
        m_next_author_id(next_author_id),
        m_author_list(author_list),
        m_book_list(book_list)
    {}
    storage_data():
           m_next_book_id(1),
           m_next_author_id(1),
           m_author_list({}),
           m_book_list({})
       {}

    int m_next_book_id;
    int m_next_author_id;
    std::vector<std::shared_ptr<Author>> m_author_list;
    std::vector<std::shared_ptr<Book>> m_book_list;
};
