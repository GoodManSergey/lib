#include<memory>
#include<string>
#include<iostream>
#include<unordered_map>
#include<vector>
#include<algorithm>
#include<assert.h>
#include<fstream>
#include<sstream>
#include"jsoncpp/json/json.h"
#include"pugixml.hpp"



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
    AUTHOR_HAS_BOOKS,
    PARSER_ERROR,
    CAN_NOT_OPEN_FILE
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
    storage_data(int next_book_id, int next_author_id, std::vector<std::shared_ptr<Author>> author_list, std::vector<std::shared_ptr<Book>> book_list):
        m_next_book_id(next_book_id), 
        m_next_author_id(next_author_id), 
        m_author_list(author_list), 
        m_book_list(book_list)
    {}
        
    int m_next_book_id;
    int m_next_author_id;
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
    public:
    virtual storage_data get_storage(const std::string& file_str) = 0;
    virtual std::string add_book(const std::string& file_str, std::shared_ptr<const Book> book) = 0;
    virtual std::string add_author(const std::string& file_str, std::shared_ptr<const Author> author) = 0;
    virtual std::string change_book(const std::string& file_str, std::shared_ptr<const Book> book) = 0;
    virtual std::string change_author(const std::string& file_str, std::shared_ptr<const Author> author) = 0;
    virtual std::string delete_book(const std::string& file_str, int book_id) = 0;
    virtual std::string delete_author(const std::string& file_str, int author_id) = 0;
};


class XmlParser: public Parser
{
    storage_data get_storage(const std::string& file_str)
    {
        pugi::xml_document doc;
        
        assert(doc.load_string(file_str.c_str()));
        pugi::xml_node root = doc.document_element();
        
        pugi::xpath_query books_query("/data/books/book");
        pugi::xpath_query authors_query("/data/authors/author");
        pugi::xpath_query next_book_id_path("/data/next_book_id");
        pugi::xpath_query next_author_id_path("/data/next_author_id");
        
        std::vector<std::shared_ptr<Author>> author_list;
        std::vector<std::shared_ptr<Book>> book_list;
        
        for (pugi::xpath_node author_query : root.select_nodes(authors_query))
        {
            pugi::xml_node author = author_query.node();
            author_list.push_back(std::shared_ptr<Author>(new Author(author.attribute("id").as_int(), author.attribute("name").value())));
        }
        
        for (pugi::xpath_node book_query : root.select_nodes(books_query))
        {
            pugi::xml_node book = book_query.node();
            
            auto author_iter = std::find_if(author_list.begin(), author_list.end(),
             [author_id = book.attribute("author_id").as_int()] (std::shared_ptr<Author>  author) -> bool { return author->get_author_id() == author_id; });
             
            assert(author_iter!=author_list.end());
            
            book_list.push_back(std::shared_ptr<Book>(new Book(book.attribute("id").as_int(), book.attribute("title").value(), (*author_iter))));
        }
  
        return {root.select_single_node(next_book_id_path).node().attribute("id").as_int(), root.select_single_node(next_author_id_path).node().attribute("id").as_int(), author_list, book_list};
    }
    
    std::string add_book(const std::string& file_str, std::shared_ptr<const Book> book)
    {
        return "";
    }
    
    std::string add_author(const std::string& file_str, std::shared_ptr<const Author> author)
    {
        return "";
    }
    
    std::string change_book(const std::string& file_str, std::shared_ptr<const Book> book)
    {
        return "";
    }
    
    std::string change_author(const std::string& file_str, std::shared_ptr<const Author> author)
    {
        return "";
    }
    
    std::string delete_book(const std::string& file_str, int book_id)
    {
        return "";
    }
    
    std::string delete_author(const std::string& file_str, int author_id)
    {
        return "";
    }
};


class JsonParser: public Parser
{
    
    storage_data get_storage(const std::string& file_str)
    {
        Json::Value root;
        Json::Reader reader;
        
        assert(reader.parse(file_str.c_str(), root ));
        
        std::vector<std::shared_ptr<Author>> author_list;
        std::vector<std::shared_ptr<Book>> book_list;
        
        for (auto author : root["authors"])
        {
            author_list.push_back(std::shared_ptr<Author>(new Author(author["id"].asInt(), author["name"].asString())));
        }
        
        for (auto book : root["books"])
        {
            auto author_iter = std::find_if(author_list.begin(), author_list.end(),
             [author_id = book["author_id"].asInt()] (std::shared_ptr<Author>  author) -> bool { return author->get_author_id() == author_id; });
             
            assert(author_iter!=author_list.end());
            
            book_list.push_back(std::shared_ptr<Book>(new Book(book["id"].asInt(), book["title"].asString(), (*author_iter))));
        }
        
        return {root["next_book_id"].asInt(), root["next_author_id"].asInt(), author_list, book_list};
    }
    
    std::string add_book(const std::string& file_str, std::shared_ptr<const Book> book)
    {
        Json::Value root;
        Json::Reader reader;
        
        assert(reader.parse(file_str.c_str(), root ));
        
        Json::Value book_json;
        book_json["id"] = book->get_book_id();
        book_json["title"] = book->get_book_title();
        book_json["author_id"] = book->get_author_id();
        
        root["books"].append(book_json);
        root["next_book_id"] = book->get_book_id() + 1;
        
        Json::FastWriter fastWriter;
        std::string output = fastWriter.write(root);
        
        return output;
    }
    
    std::string add_author(const std::string& file_str, std::shared_ptr<const Author> author)
    {
        Json::Value root;
        Json::Reader reader;
        
        assert(reader.parse(file_str.c_str(), root ));
        
        Json::Value author_json;
        author_json["id"] = author->get_author_id();
        author_json["name"] = author->get_name();
        
        root["authors"].append(author_json);
        root["next_author_id"] = author->get_author_id() + 1;
        
        Json::FastWriter fastWriter;
        std::string output = fastWriter.write(root);
        
        return output;
    }
    
    std::string change_book(const std::string& file_str, std::shared_ptr<const Book> book)
    {
        Json::Value root;
        Json::Reader reader;
        
        assert(reader.parse(file_str.c_str(), root ));
        
        for (auto& book_json : root["books"])
        {
            if (book_json["id"] == book->get_book_id())
            {
                book_json["title"] = book->get_book_title();
                book_json["author_id"] = book->get_author_id();
                break;
            }   
        }
        
        Json::FastWriter fastWriter;
        std::string output = fastWriter.write(root);
        
        return output;
    }
    
    std::string change_author(const std::string& file_str, std::shared_ptr<const Author> author)
    {
        Json::Value root;
        Json::Reader reader;
        
        assert(reader.parse(file_str.c_str(), root ));
        
        for (auto& author_json : root["authors"])
        {
            if (author_json["id"] == author->get_author_id())
            {
                author_json["name"] = author->get_name();
                break;
            }   
        }
        
        Json::FastWriter fastWriter;
        std::string output = fastWriter.write(root);
        
        return output;
    }
    
    std::string delete_book(const std::string& file_str, int book_id)
    {
        Json::Value root;
        Json::Reader reader;
        
        Json::Value new_books;
         
        assert(reader.parse(file_str.c_str(), root ));
        
        for (auto& book_json : root["books"])
        {
            if (book_json["id"].asInt() == book_id)
            {   
                continue;
            }
            new_books.append(book_json);
        }
        
        root["books"] = new_books;
        
        Json::FastWriter fastWriter;
        std::string output = fastWriter.write(root);
        
        return output;
    }
    
    std::string delete_author(const std::string& file_str, int author_id)
    {
        Json::Value root;
        Json::Reader reader;
        
        Json::Value new_authors;
         
        assert(reader.parse(file_str.c_str(), root ));
        
        for (auto& author_json : root["authors"])
        {
            if (author_json["id"].asInt() == author_id)
            {   
                continue;
            }
            new_authors.append(author_json);
        }
        
        root["authors"] = new_authors;
        
        Json::FastWriter fastWriter;
        std::string output = fastWriter.write(root);
        
        return output;
    }
};


class FileStorage: public Storage
{
   public:
    FileStorage(std::unique_ptr<Parser> parser, std::string storage_path): 
        pm_parser(std::move(parser)), 
        m_storage_path(storage_path)
    {}
    
    std::string get_string_from_file()
    {
        std::ifstream file(this->m_storage_path);
        std::stringstream buffer; 
        
        if (file.is_open())
        {
            buffer << file.rdbuf();
            file.close();
        }
        else
        {
            std::ifstream file_tmp(this->m_storage_path + ".tmp");
            
            assert(file_tmp.is_open());
            
            buffer << file_tmp.rdbuf();
            file_tmp.close();
            
            bool rename_status = std::rename((this->m_storage_path + ".tmp").c_str(), this->m_storage_path.c_str());
        
            assert(!rename_status);
        }       
        
        return buffer.str();
    }
    
    result_code set_data_to_file(const std::string& str_data)
    {
        std::ofstream file(this->m_storage_path + ".tmp");
        
        assert(file.is_open());
        file << str_data;
        file.close();
        
        bool rename_status = std::rename((this->m_storage_path + ".tmp").c_str(), this->m_storage_path.c_str());
        
        assert(!rename_status);
        
        return result_code::OK;
    }
    
    storage_data get_storage() 
    {
        return pm_parser->get_storage(this->get_string_from_file());
    }
        
    result_code add_book(std::shared_ptr<const Book> book)
    {   
        std::string new_data = pm_parser->add_book(this->get_string_from_file(), book);
        
        return this->set_data_to_file(new_data);
    }
    
    result_code add_author(std::shared_ptr<const Author> author)
    {
        std::string new_data = pm_parser->add_author(this->get_string_from_file(), author);
        
        return this->set_data_to_file(new_data);
    }
    
    result_code change_book(std::shared_ptr<const Book> book)
    {
        std::string new_data = pm_parser->change_book(this->get_string_from_file(), book);
        
        return this->set_data_to_file(new_data);
    }
    
    result_code change_author(std::shared_ptr<const Author> author)
    {
        std::string new_data = pm_parser->change_author(this->get_string_from_file(), author);
        
        return this->set_data_to_file(new_data);
    }
    
    result_code delete_book(int book_id)
    {
        std::string new_data = pm_parser->delete_book(this->get_string_from_file(), book_id);
        
        return this->set_data_to_file(new_data);
    }
    
    result_code delete_author(int author_id)
    {
        std::string new_data = pm_parser->delete_author(this->get_string_from_file(), author_id);
        
        return this->set_data_to_file(new_data);
    }
    
private:
    std::unique_ptr<Parser> pm_parser;
    std::string m_storage_path;
};


class Library
{
    public:
    Library(std::unique_ptr<Storage> storage): 
        pm_storage(std::move(storage))
    {
        auto gotten_storage = pm_storage->get_storage();
        
        this->m_next_book_id = gotten_storage.m_next_book_id;
        this->m_next_author_id = gotten_storage.m_next_author_id;
        
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
    Library lib(std::unique_ptr<Storage>(new FileStorage(std::unique_ptr<Parser>(new XmlParser()), "FileStore.xml")));
    auto book = lib.get_book_by_id(1).m_object;
    std::cout<<book->get_book_title()<<std::endl<<book->get_author()->get_name()<<std::endl;
    /*
    auto add_author = lib.add_author(std::unique_ptr<Author>(new Author(1, "new author")));
    auto author = lib.get_author_by_id(1).m_object;
    auto add_book = lib.add_book(std::unique_ptr<Book>(new Book(1, "new book", author)));
    
    auto change_book = lib.change_book(std::unique_ptr<Book>(new Book(1, "The new book", author)));
    auto change_author = lib.change_author(std::unique_ptr<Author>(new Author(1, "The new author")));
    
    auto delete_book = lib.delete_book(10);
    if (delete_book == result_code::OK)
        std::cout<<"OK"<<std::endl;
    auto delete_author = lib.delete_author(3);
    if (delete_author == result_code::OK)
        std::cout<<"OK"<<std::endl;
        */
}
