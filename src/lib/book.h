#pragma once
#include"author.h"
#include<string>
#include<memory>

class Book
{
    public:
    Book(int book_id, const std::string& title, std::shared_ptr<const Author> author);
    int get_book_id() const;
    const std::string& get_book_title() const;
    std::shared_ptr<const Author> get_author() const;
    int get_author_id() const;
    void set_book_id(int new_book_id);
    void set_title(const std::string& new_title);
    void set_author(const std::shared_ptr<Author> new_author);

    private:
    int m_book_id;
    std::string m_title;
    std::shared_ptr<const Author> m_author;
};
