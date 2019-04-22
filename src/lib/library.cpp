#include"library.h"


Library::Library(std::unique_ptr<Storage> storage):
        pm_storage(std::move(storage))
    {
        auto gotten_storage = pm_storage->get_storage();

        m_next_book_id = gotten_storage.m_next_book_id;
        m_next_author_id = gotten_storage.m_next_author_id;

        for (auto author : gotten_storage.m_author_list)
        {
            auto insert_result = m_author_list.insert({author->get_author_id(), author});
            assert(insert_result.second);
            m_authors_books[author->get_author_id()];
        }

        for (auto book : gotten_storage.m_book_list)
        {
            auto insert_result = m_book_list.insert({book->get_book_id(), book});

            assert(insert_result.second);

            m_authors_books[insert_result.first->second->get_author_id()].push_back(insert_result.first->first);
        }
    };

result<int> Library::add_author(std::unique_ptr<Author> author)
    {
        if (!author)
        {
            return result_code::INVALID_INPUT_PARAMS;
        }

        author->set_author_id(m_next_author_id++);

        std::shared_ptr<Author> shared_author = std::move(author);

        auto storage_result = pm_storage->add_author(shared_author);
        if (storage_result != result_code::OK)
        {
            return storage_result;
        }

        auto insert_result = m_author_list.insert({shared_author->get_author_id(), shared_author});
        assert(insert_result.second);

        return insert_result.first->first;
    };

result<std::shared_ptr<const Author>> Library::get_author_by_id(int author_id)
    {
        auto iter = m_author_list.find(author_id);

        if (iter != m_author_list.end())
        {
            return {iter->second};
        }

        return {result_code::AUTHOR_NOT_FOUND};
    };

result<std::vector<std::shared_ptr<const Book>>> Library::get_authors_books(int author_id)
    {
        auto iter = m_authors_books.find(author_id);

        if (iter == m_authors_books.end())
        {
            return result_code::AUTHOR_NOT_FOUND;
        }

        std::vector<std::shared_ptr<const Book>> authors_books;

        for (int book_id : iter->second)
        {
            authors_books.push_back(m_book_list[book_id]);
        }

        return authors_books;
    };

result<int> Library::add_book(std::unique_ptr<Book> book)
    {
        if (!book)
        {
            return result_code::INVALID_INPUT_PARAMS;
        }

        if (book->get_author_id() <= 0)
        {
            return result_code::AUTHOR_NOT_PRESENT;
        }

        auto iter_author = m_author_list.find(book->get_author_id());
        if (iter_author == m_author_list.end())
        {
            return result_code::AUTHOR_NOT_FOUND;
        }

        book->set_author(iter_author->second);
        book->set_book_id(m_next_book_id++);

        std::shared_ptr<Book> shared_book = std::move(book);

        auto storage_result = pm_storage->add_book(shared_book);
        if (storage_result != result_code::OK)
        {
            return storage_result;
        }

        auto insert_result = m_book_list.insert({shared_book->get_book_id(), shared_book});

        assert(insert_result.second);

        m_authors_books[insert_result.first->second->get_author_id()].push_back(insert_result.first->first);

        return insert_result.first->first;
    };

result<std::shared_ptr<const Book>> Library::get_book_by_id(int book_id)
    {
        auto iter = m_book_list.find(book_id);

        if (iter != m_book_list.end())
        {
            return std::const_pointer_cast<const Book>(iter->second);
        }

        return result_code::BOOK_NOT_FOUND;
    };

result<int> Library::change_author(std::unique_ptr<Author> author)
    {
        auto iter_author = m_author_list.find(author->get_author_id());

        if (iter_author == m_author_list.end())
        {
            return result_code::AUTHOR_NOT_FOUND;
        }

        std::shared_ptr<Author> shared_author = std::move(author);

        auto storage_result = pm_storage->change_author(shared_author);
        if (storage_result != result_code::OK)
        {
            return storage_result;
        }

        iter_author->second = shared_author;

        for (int book_id : m_authors_books[iter_author->first])
        {
            auto new_book_ptr = std::make_shared<Book>(*m_book_list[book_id]);
            new_book_ptr->set_author(iter_author->second);
            m_book_list[book_id] = new_book_ptr;
        }

        return iter_author->first;
    };

result<int> Library::change_book(std::unique_ptr<Book> book)
    {
        if (!book)
        {
            return result_code::INVALID_INPUT_PARAMS;
        }

        if (book->get_author_id() <= 0)
        {
            return result_code::AUTHOR_NOT_PRESENT;
        }

        auto iter_author = m_author_list.find(book->get_author_id());

        if (iter_author == m_author_list.end())
        {
            return result_code::AUTHOR_NOT_FOUND;
        }

        auto iter_book = m_book_list.find(book->get_book_id());

        if (iter_book == m_book_list.end())
        {
            return result_code::BOOK_NOT_FOUND;
        }

	    book->set_author(iter_author->second);

        std::shared_ptr<Book> shared_book = std::move(book);

        auto storage_result = pm_storage->change_book(shared_book);
        if (storage_result != result_code::OK)
        {
            return storage_result;
        }

        iter_book->second = shared_book;

        return iter_book->first;
    };

result_code Library::delete_author(int author_id)
    {
        auto iter_author = m_author_list.find(author_id);

        if (iter_author == m_author_list.end())
        {
            return result_code::AUTHOR_NOT_FOUND;
        }

        auto authors_books = m_authors_books.find(author_id);

        if (authors_books != m_authors_books.end())
        if (!authors_books->second.empty())
        {
            return result_code::AUTHOR_HAS_BOOKS;
        }

        auto storage_result = pm_storage->delete_author(author_id);
        if (storage_result != result_code::OK)
        {
            return storage_result;
        }

        m_authors_books.erase(author_id);

        m_author_list.erase(author_id);

        return result_code::OK;
    };

result_code Library::delete_book(int book_id)
    {
        auto iter_book = m_book_list.find(book_id);

        if (iter_book == m_book_list.end())
        {
            return result_code::BOOK_NOT_FOUND;
        }

        auto storage_result = pm_storage->delete_book(book_id);
        if (storage_result != result_code::OK)
        {
            return storage_result;
        }

        assert(iter_book->second->get_author_id() > 0);

        auto & author_books = m_authors_books[iter_book->second->get_author_id()];
        author_books.erase(std::remove(author_books.begin(), author_books.end(), iter_book->first), author_books.end());

        m_book_list.erase(iter_book);

        return result_code::OK;
    };
