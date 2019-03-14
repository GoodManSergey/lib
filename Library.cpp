#include<memory>
#include<string>
#include<iostream>
#include<unordered_map>
#include<vector>
#include<algorithm>
#include<assert.h>



enum class result_code: int
{
    OK,
    BOOK_NOT_FOUND,
    AUTHOR_NOT_FOUND,
    INVALID_INPUT_PARAMS,
    INTERNAL_ERROR,
    AUTHOR_NOT_PRESENT,
    BOOK_NOT_PRESENT,
    STORAGE_ERROR,
    AUTHOR_HAS_BOOKS
};


template<typename T>
struct result
{
    result(result_code code):
        m_code(code)
    {}

    result(const T& object):
        m_code(result_code::OK),
        m_object(object)
    {}

    result_code m_code;
    T m_object;
};


class Author
{
    public:
    Author(int id, const std::string& name): m_author_id(id), m_name(name)
    {}
    
    int get_author_id() const {return m_author_id;}
    const std::string& get_name() const {return m_name;}

    void set_author_id(int id)
    {
        this->m_author_id = id;
    }

    void set_name(const std::string& name)
    {   
        this->m_name = name;
    }

    private:
    std::string m_name;
    int m_author_id;

};


class Book
{   
    public:
    Book(int book_id, const std::string& title, std::shared_ptr<const Author> author): 
        m_book_id(book_id), m_title(title), m_author(author)
    {}        
    
    int get_book_id() const {return m_book_id;}
    const std::string& get_book_title() const {return m_title;}
    std::shared_ptr<const Author> get_author() const {return m_author;}

    int get_author_id() const
    {
        if (m_author)
        {
            return m_author->get_author_id();
        }
        else
        {
            return -1;
        }
    }

    void set_book_id(int new_book_id)
    {
        this->m_book_id = new_book_id;
    }

    void set_title(const std::string& new_title)
    {
        this->m_title = new_title;
    }

    void set_author(const std::shared_ptr<Author> new_author)
    {
        this->m_author = new_author;
    }

    private:
    int m_book_id;
    std::string m_title;
    std::shared_ptr<const Author> m_author;    
};


struct storage_data
{
	
    int next_book_id;
    int next_author_id;
    std::vector<std::shared_ptr<Author>> m_author_list;
    std::vector<std::shared_ptr<Book>> m_book_list;
};


class Storage
{
    public:
    virtual storage_data get_storage() = 0;
    virtual result_code add_book(std::shared_ptr<const Book> book) = 0;
    virtual result_code add_author(std::shared_ptr<const Author> author) = 0;
    virtual result_code change_book(std::shared_ptr<const Book> book) = 0;
    virtual result_code change_author(std::shared_ptr<const Author> author) = 0;
    virtual result_code delete_book(int book_id) = 0;
    virtual result_code delete_author(int author_id) = 0;
};


class Parser
{

};


class FileStorage: public Storage
{
   public:
    FileStorage(std::unique_ptr<Parser> parser, std::string storage_path): pm_parser(std::move(parser)), m_storage_path(storage_path)
    {
    }
    
    storage_data get_storage() {};
    
    result_code add_book(std::shared_ptr<const Book> book)
    {
        return result_code::OK;
    }
    
    result_code add_author(std::shared_ptr<const Author> author)
    {
        return result_code::OK;
    }
    
    result_code change_book(std::shared_ptr<const Book> book)
    {
        return result_code::OK;
    }
    
    result_code change_author(std::shared_ptr<const Author> author)
    {
        return result_code::OK;
    }
    
    result_code delete_book(int book_id)
    {
        return result_code::OK;
    }
    
    result_code delete_author(int author_id)
    {
        return result_code::OK;
    }
    
private:
    std::unique_ptr<Parser> pm_parser;
    std::string m_storage_path;
};


class Library
{
    public:
    Library(std::unique_ptr<Storage> storage): pm_storage(std::move(storage)), m_next_author_id(1), m_next_book_id(1)
    {
    }
    
    result<int> add_author(std::unique_ptr<Author> author)
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
    }

    result<std::shared_ptr<const Author>> get_author_by_id(int author_id)
    {
        auto iter = m_author_list.find(author_id);
        
        if (iter != m_author_list.end())
        {   
            return {iter->second};
        }
        
        return {result_code::AUTHOR_NOT_FOUND};
    }

    result<std::vector<std::shared_ptr<const Book>>> get_authors_books(int author_id)
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
    }
    
    result<int> add_book(std::unique_ptr<Book> book)
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
    }
    
    result<std::shared_ptr<const Book>> get_book_by_id(int book_id)
    {
        auto iter = m_book_list.find(book_id);
        
        if (iter != m_book_list.end())
        {   
            return std::const_pointer_cast<const Book>(iter->second);
        }
        
        return result_code::BOOK_NOT_FOUND;
    }
    
    result<int> change_author(std::unique_ptr<Author> author)
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
    }

    result<int> change_book(std::unique_ptr<Book> book)
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
    }
    
    result_code delete_author(int author_id)
    {
        auto iter_author = m_author_list.find(author_id);
        
        if (iter_author == m_author_list.end())
        {
            return result_code::AUTHOR_NOT_FOUND;
        }

        auto authors_books = m_authors_books.find(author_id);
        
        if (authors_books->second.empty())
        {
            return result_code::AUTHOR_HAS_BOOKS;
        }
        
        auto storage_result = pm_storage->delete_author(author_id);
        if (storage_result != result_code::OK)
        {
            return storage_result;
        }
        
        m_authors_books.erase(author_id);
        
        m_author_list.erase(iter_author);
        
        return result_code::OK;
    }
    
    result_code delete_book(int book_id)
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
    }

    private:
    int m_next_author_id;
    int m_next_book_id;
    std::unordered_map<int, std::shared_ptr<Author>> m_author_list;
    std::unordered_map<int, std::shared_ptr<Book>> m_book_list;
    std::unordered_map<int, std::vector<int>> m_authors_books;
    std::unique_ptr<Storage> pm_storage;
    
};


int main()
{
    Library lib(std::unique_ptr<Storage>(new FileStorage(std::unique_ptr<Parser>(new Parser()), "/file/path")));

    auto Book_ptr = lib.get_book_by_id(1).m_object;
    auto NewBook_ptr = std::unique_ptr<Book>(new Book(*Book_ptr));
    NewBook_ptr->set_title("foo");


    
    /*std::unique_ptr<Author> author(new Author(1, "Oruell"));

    //Добавляем автора
    int auth_id = lib.add_author(std::move(author));
    
    bool have_books;
    std::vector<std::shared_ptr<const Book>> auth_books = lib.get_authors_books(auth_id, have_books);
    if (have_books)
    {
        std::cout<<auth_books.size()<<std::endl;
    }
    else
    {
        std::cout<<"No books"<<std::endl;
    }
    //Получаем несущ автора
    std::shared_ptr<const Author> none_author = lib.get_author_by_id(100);
    std::cout<<"EMPTY AUTHOR: "<<none_author<<std::endl;
    
    //Получаем автора
    std::shared_ptr<const Author> author_2 = lib.get_author_by_id(auth_id);

    std::unique_ptr<Book> book(new Book(1, "1984", author_2));
    //Добавляем книгу
    int book_id = lib.add_book(std::move(book));
    
    auth_books = lib.get_authors_books(auth_id, have_books);
    if (have_books)
    {
        std::cout<<"Books: "<<auth_books.size()<<std::endl;
    }
    else
    {
        std::cout<<"No books"<<std::endl;
    }
    
    //Получаем книгу
    std::shared_ptr<const Book> gotten_book= lib.get_book_by_id(book_id);
    
    std::unique_ptr<Author> author_3(new Author(1, "Ne Pushkin"));
    //Изменяем автора
    lib.change_author(std::move(author_3));

    //Изменяеи книгу(название)
    std::unique_ptr<Book> new_book(new Book(1, "ne ev on", author_2));
    lib.change_book(std::move(new_book));

    //Получаем ту же книгу
    std::shared_ptr<const Book> gotten_book_2 = lib.get_book_by_id(book_id);
    
    
    
    std::cout<<gotten_book->get_book_id()<<std::endl;
    std::cout<<gotten_book->get_author()->get_name()<<std::endl;
    std::cout<<gotten_book->get_book_title()<<std::endl;
    std::cout<<"-------------------"<<std::endl;
    std::cout<<gotten_book_2->get_book_id()<<std::endl;
    std::cout<<gotten_book_2->get_author()->get_name()<<std::endl;
    std::cout<<gotten_book_2->get_book_title()<<std::endl; */


    return 0;
}
