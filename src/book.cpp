#include"book.h"

Book::Book(int book_id, const std::string& title, std::shared_ptr<const Author> author):
        m_book_id(book_id), m_title(title), m_author(author)
    {};
    
int Book::get_book_id() const {return m_book_id;};

const std::string& Book::get_book_title() const {return m_title;};

std::shared_ptr<const Author> Book::get_author() const {return m_author;};

int Book::get_author_id() const
{
	if (m_author)
	{
		return m_author->get_author_id();
	}
	else
	{
		return -1;
	}
};

void Book::set_book_id(int new_book_id)
{
	m_book_id = new_book_id;
};

void Book::set_title(const std::string& new_title)
{
	m_title = new_title;
};

void Book::set_author(const std::shared_ptr<Author> new_author)
{
	m_author = new_author;
};
