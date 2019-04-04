#include<memory>
#include<string>
#include<iostream>
#include<unordered_map>
#include<vector>
#include<algorithm>
#include<assert.h>
#include<fstream>
#include<sstream>
#include<functional>
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
    OPEN_FILE_ERROR
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
    virtual std::string set_empty_tmpl() = 0;
    virtual result<storage_data> get_storage(const std::string& file_str) = 0;
    virtual result<std::string> add_book(const std::string& file_str, std::shared_ptr<const Book> book) = 0;
    virtual result<std::string> add_author(const std::string& file_str, std::shared_ptr<const Author> author) = 0;
    virtual result<std::string> change_book(const std::string& file_str, std::shared_ptr<const Book> book) = 0;
    virtual result<std::string> change_author(const std::string& file_str, std::shared_ptr<const Author> author) = 0;
    virtual result<std::string> delete_book(const std::string& file_str, int book_id) = 0;
    virtual result<std::string> delete_author(const std::string& file_str, int author_id) = 0;
};


class XmlParser: public Parser
{
    public:
    std::string set_empty_tmpl()
    {   
        std::string file_tmpl = "<data><books></books> <authors></authors> <next_book_id id='1' /> <next_author_id id='1' /> </data>";
        this->m_doc.load(file_tmpl.c_str());

        std::stringstream buffer;
    	this->m_doc.save(buffer);
    	
    	return std::move(buffer.str());
    }
    
    result<storage_data> get_storage(const std::string& file_str)
    {        
        if (!this->m_doc.load_string(file_str.c_str()))
        {
        	return result_code::PARSER_ERROR;
        }

        pugi::xml_node root = this->m_doc.document_element();

        pugi::xpath_query books_query("/data/books");
        pugi::xpath_query authors_query("/data/authors");
        pugi::xpath_query next_book_id_path("/data/next_book_id");
        pugi::xpath_query next_author_id_path("/data/next_author_id");
        
        std::vector<std::shared_ptr<Author>> author_list{};
        std::vector<std::shared_ptr<Book>> book_list{};
        
        pugi::xml_node authors_node = root.select_single_node(authors_query).node();
        if (authors_node == nullptr)
        {
        	std::cout<<"no authors node"<<std::endl;
        	return result_code::PARSER_ERROR;
        }

        for (auto author_child = authors_node.child("author"); author_child; author_child = author_child.next_sibling("author"))
        {
        	auto author_id = author_child.attribute("id");
        	auto author_name = author_child.attribute("name");
        	if(!(author_id and author_name))
        	{
        		continue;
        	}
        	
        	int author_id_int = author_id.as_int(-1);
        	
        	if (author_id_int == -1)
        	{
        	    continue;
        	}
        	
        	author_list.push_back(std::shared_ptr<Author>(new Author(author_id.as_int(), author_name.value())));
        }
        
        pugi::xml_node books_node = root.select_single_node(books_query).node();
        if (books_node == nullptr)
        {
        	std::cout<<"no books node"<<std::endl;
        	return result_code::PARSER_ERROR;
        }

        for (auto book_child = books_node.child("book"); book_child; book_child = book_child.next_sibling("book"))
        {
        	auto book_author_id = book_child.attribute("author_id");
        	auto book_id = book_child.attribute("id");
        	auto title = book_child.attribute("title");

        	if (!(book_author_id and book_id and title))
        	{
        		continue;
        	}
            
            int book_author_id_int = book_author_id.as_int(-1);
            int book_id_int = book_id.as_int(-1);
            
            if (book_author_id_int == -1 or book_id_int == -1)
            {
                continue;
            }
            
        	auto find_author = [author_id = book_author_id_int] (std::shared_ptr<Author>  author) -> bool
        			{ return author->get_author_id() == author_id; };
        	auto author_iter = std::find_if(author_list.begin(), author_list.end(), find_author);

        	if (author_iter==author_list.end())
        	{
        		continue;
        	}

        	book_list.push_back(std::shared_ptr<Book>(new Book(book_id_int, title.value(), (*author_iter))));
        }

        pugi::xml_node next_book_id_node = root.select_single_node(next_book_id_path).node();
        if (next_book_id_node == nullptr)
        {
        	return result_code::PARSER_ERROR;
        }

        pugi::xml_node next_author_id_node = root.select_single_node(next_author_id_path).node();
        if (next_book_id_node == nullptr)
        {   
            std::cout<<"no next_book_id node"<<std::endl;
        	return result_code::PARSER_ERROR;
       	}

        auto next_book_id = next_book_id_node.attribute("id");
        auto next_author_id = next_author_id_node.attribute("id");

        if (!(next_book_id and next_author_id))
        {
            std::cout<<"no next_author_id node"<<std::endl;
        	return result_code::PARSER_ERROR;
        }

        storage_data data {next_book_id.as_int(), next_author_id.as_int(), author_list, book_list};

        return std::move(data);
    }
    
    result<std::string> add_node(const std::string& file_str, const pugi::xpath_query& nodes_path, 
                                 const pugi::xpath_query& next_id_path, std::function<void(pugi::xml_node)> node_filler)
    {
        pugi::xml_node root = this->m_doc.document_element();
        
        auto node = root.select_single_node(nodes_path).node();
        
        auto next_id_node = root.select_single_node(next_id_path).node();
        
        
        if (node == nullptr or next_id_node == nullptr)
        {
            return result_code::PARSER_ERROR;
        }
        
        auto next_id_attr = next_id_node.attribute("id");
        if (!next_id_attr)
        {
            return result_code::PARSER_ERROR;
        }
        
        node_filler(node);
        
        next_id_attr.set_value(next_id_attr.as_int() + 1);
        
        std::stringstream buffer;
    	this->m_doc.save(buffer);

        return std::move(buffer.str());
    }
    
    result<std::string> add_book(const std::string& file_str, std::shared_ptr<const Book> book)
    {        
        pugi::xpath_query books_query("/data/books");
    	pugi::xpath_query next_book_id_path("/data/next_book_id");
    	
    	auto book_node_filler = [book](pugi::xml_node books_node)-> void
    	                                {
    	                                    pugi::xml_node book_node = books_node.append_child("book");

    	                                    book_node.append_attribute("id") = book->get_book_id();
    	                                    book_node.append_attribute("title") = book->get_book_title().c_str();
    	                                    book_node.append_attribute("author_id") = book->get_author_id();
    	                                };
        return std::move(this->add_node(file_str, books_query, next_book_id_path, book_node_filler));   
    }
    
    result<std::string> add_author(const std::string& file_str, std::shared_ptr<const Author> author)
    {    	
    	pugi::xpath_query authors_query("/data/authors");
    	pugi::xpath_query next_author_id_path("/data/next_author_id");
    	
    	auto author_node_filler = [author](pugi::xml_node authors_node)-> void
    	                                {
    	                                    pugi::xml_node author_node = authors_node.append_child("author");

    	                                    author_node.append_attribute("id") = author->get_author_id();
    	                                    author_node.append_attribute("name") = author->get_name().c_str();
    	                                };
        return std::move(this->add_node(file_str, authors_query, next_author_id_path, author_node_filler));
    }
    
    result<std::string> change_node(const std::string& file_str, const pugi::xpath_query& nodes_path,
                                     std::function<void(pugi::xml_node)> node_filler, std::function<bool(pugi::xml_node)> node_finder)
    {
        pugi::xml_node root = this->m_doc.document_element();
        
        auto nodes = root.select_single_node(nodes_path).node();
        
        if (nodes == nullptr)
        {
            return result_code::PARSER_ERROR;
        }
        
        auto change_node = nodes.find_child(node_finder);
        
        if (change_node == nullptr)
        {
            return result_code::PARSER_ERROR;
        }
        
        node_filler(change_node);
        
        std::stringstream buffer;
    	this->m_doc.save(buffer);

        return std::move(buffer.str());
    }
    
    result<std::string> change_book(const std::string& file_str, std::shared_ptr<const Book> book)
    {
    	pugi::xpath_query books_query("/data/books");
    	auto find_book_by_id = [book_id = book->get_book_id()] (pugi::xml_node node) -> bool {return node.attribute("id").as_int() == book_id; };
    	
    	auto book_node_filler = [book](pugi::xml_node book_node)-> void
    	                                {
    	                                    book_node.attribute("title").set_value(book->get_book_title().c_str());
    	                                    book_node.attribute("author_id").set_value(book->get_author_id());
    	                                };
    	                                
        return std::move(this->change_node(file_str, books_query, book_node_filler, find_book_by_id));
    }
    
    result<std::string> change_author(const std::string& file_str, std::shared_ptr<const Author> author)
    {
        pugi::xpath_query authors_query("/data/authors");
    	auto find_author_by_id = [author_id = author->get_author_id()] (pugi::xml_node node) -> bool {return node.attribute("id").as_int() == author_id; };
    	
    	auto author_node_filler = [author](pugi::xml_node author_node)-> void
    	                                {
    	                                    author_node.attribute("name").set_value(author->get_name().c_str());
    	                                };
    	                                
        return std::move(this->change_node(file_str, authors_query, author_node_filler, find_author_by_id));
    }
    
    result<std::string> delete_node_by_id(const std::string& file_str, const pugi::xpath_query& nodes_path, int node_id)
    {
        pugi::xml_node root = this->m_doc.document_element();
        
        auto nodes = root.select_single_node(nodes_path).node();
        
        if (nodes == nullptr)
        {
            return result_code::PARSER_ERROR;
        }
        
        auto find_node_by_id = [node_id] (pugi::xml_node node) -> bool {return node.attribute("id").as_int() == node_id; };
        
        auto del_node = nodes.find_child(find_node_by_id);
        
        if (del_node == nullptr)
        {
            return result_code::PARSER_ERROR;
        }
        
        nodes.remove_child(del_node);
        
        std::stringstream buffer;
    	this->m_doc.save(buffer);

    	return std::move(buffer.str());
    }
    
    result<std::string> delete_book(const std::string& file_str, int book_id)
    {
    	pugi::xpath_query books_query("/data/books");
    	
    	return std::move(this->delete_node_by_id(file_str, books_query, book_id));
    }
    
    result<std::string> delete_author(const std::string& file_str, int author_id)
    {
    	pugi::xpath_query authors_query("/data/authors");
    	
    	return std::move(this->delete_node_by_id(file_str, authors_query, author_id));
    }
    
    private:
        pugi::xml_document m_doc;
};


class JsonParser: public Parser
{   
    public:
    std::string set_empty_tmpl()
    {
        Json::Reader reader;
        std::string file_tmpl = "{\"authors\":[],\"books\":[],\"next_author_id\":1,\"next_book_id\": 1}";
        
        reader.parse(file_tmpl.c_str(), this->m_root);
        
        Json::FastWriter fastWriter;
        std::string output = fastWriter.write(this->m_root);
        
        return output;
    }
    
    result<storage_data> get_storage(const std::string& file_str)
    {
        Json::Reader reader;
        
        if (!reader.parse(file_str.c_str(), this->m_root ))
        {
            return result_code::PARSER_ERROR;
        }
        
        std::vector<std::shared_ptr<Author>> author_list;
        std::vector<std::shared_ptr<Book>> book_list;
        
        if (!this->m_root.isMember("authors"))
        {
            std::cout<<"no authors error"<<std::endl;
            return result_code::PARSER_ERROR;
        }
        
        auto authors = this->m_root["authors"];
        
        for (auto author : authors)
        {   
            try
            {
                if (!(author.isMember("id") and author.isMember("name")))
                {
                    continue;
                }
            }
            catch(Json::Exception const&)
            {
                continue;
            }
            
            auto author_id = author["id"];
            auto author_name = author["name"];
            
            int author_id_int;
            std::string author_name_str;
            
            try
            {
                author_id_int = author_id.asInt();
                author_name_str = author_name.asString();
            }       
            catch(Json::Exception const&)
            {
                continue;
            }
            
            author_list.push_back(std::shared_ptr<Author>(new Author(author_id_int, author_name_str)));
        }
        
        if (!this->m_root.isMember("books"))
        {
            std::cout<<"no books error"<<std::endl;
            return result_code::PARSER_ERROR;
        }
        
        auto books = this->m_root["books"];
        
        for (auto book : books)
        {
            try
            {
                if (!(book.isMember("id") and book.isMember("title") and book.isMember("author_id")))
                {
                    continue;
                }
            }
            catch(Json::Exception const&)
            {
                continue;
            }
            
            auto book_id = book["id"];
            auto book_author_id = book["author_id"];
            auto book_title = book["title"];
            
            int book_id_int;
            int book_author_id_int;
            std::string book_title_str;
            
            try
            {   
                book_id_int = book_id.asInt();
                book_author_id_int = book_author_id.asInt();
                book_title_str = book_title.asString();
            }       
            catch(Json::Exception const&)
            {
                continue;
            }
            
            auto author_iter = std::find_if(author_list.begin(), author_list.end(),
             [book_author_id_int] (std::shared_ptr<Author>  author) -> bool { return author->get_author_id() == book_author_id_int; });
             
            if (author_iter==author_list.end())
            {
                continue;
            }
            
            book_list.push_back(std::shared_ptr<Book>(new Book(book_id_int, book_title_str, (*author_iter))));
        }
        
        if (!this->m_root.isMember("next_book_id"))
        {
            return result_code::PARSER_ERROR;
        }
        
        if (!this->m_root.isMember("next_author_id"))
        {
            return result_code::PARSER_ERROR;
        }
        
        auto next_book_id = this->m_root["next_book_id"];
        auto next_author_id = this->m_root["next_author_id"];
        
        int next_book_id_int;
        int next_author_id_int;
        
        try
        {
            next_book_id_int = next_book_id.asInt();
            next_author_id_int = next_author_id.asInt();
        }
        catch(Json::Exception const&)
        {
            return result_code::PARSER_ERROR;
        }
        
        storage_data data {next_book_id_int, next_author_id_int, author_list, book_list};
        return std::move(data);
    }
    
    result<std::string> add_book(const std::string& file_str, std::shared_ptr<const Book> book)
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
    
    result<std::string> add_author(const std::string& file_str, std::shared_ptr<const Author> author)
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
    
    result<std::string> change_book(const std::string& file_str, std::shared_ptr<const Book> book)
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
    
    result<std::string> change_author(const std::string& file_str, std::shared_ptr<const Author> author)
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
    
    result<std::string> delete_book(const std::string& file_str, int book_id)
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
    
    result<std::string> delete_author(const std::string& file_str, int author_id)
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
    
    private:
    Json::Value m_root;
};


class FileStorage: public Storage
{
   public:
    FileStorage(std::unique_ptr<Parser> parser, std::string storage_path): 
        pm_parser(std::move(parser)), 
        m_storage_path(storage_path)
    {}
    
    result<std::string> get_string_from_file()
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
            
            if (!file_tmp.is_open())
            {
                return result_code::OPEN_FILE_ERROR;
            }
            
            buffer << file_tmp.rdbuf();
            file_tmp.close();
            
            bool rename_status = std::rename((this->m_storage_path + ".tmp").c_str(), this->m_storage_path.c_str());
        
            assert(!rename_status);
        }       
        
        return buffer.str();
    }
    
    result_code make_file(const std::string& str_data, const std::string& postfix="")
    {
        std::ofstream file(this->m_storage_path + postfix);
        
       if (!file.is_open())
       {
    	   return result_code::OPEN_FILE_ERROR;
       }
        
        file << str_data;
        file.close();

        return result_code::OK;
    }
    
    result_code set_data_to_file(const std::string& str_data)
    {   
        std::string tmp_postfix = ".tmp";

        result_code make_file_res = this->make_file(str_data, tmp_postfix);
        
        if (make_file_res != result_code::OK)
        {
        	return make_file_res;
        }

        bool rename_status = std::rename((this->m_storage_path + tmp_postfix).c_str(), this->m_storage_path.c_str());
        
        assert(!rename_status);
        
        return result_code::OK;
    }
    
    storage_data make_tmpl_file()
    {
    	std::string file_tmpl = this->pm_parser->set_empty_tmpl(); //получаем паустой шаблон и заполняем его в парсер
    	result_code tmpl_file_res = this->make_file(file_tmpl); //записываем его в файл, без tmp, т.к. файл уже битый
    	assert(tmpl_file_res == result_code::OK);//если не вышло записать, то брасаем ошибку
    	storage_data empty_data {1,1, {}, {}};
    	return std::move(empty_data);//возвращаем пустую либу
    }

    storage_data get_storage() 
    {   
        result<std::string> get_file_res = this->get_string_from_file(); //пытаемся получить файл

        if (get_file_res.m_code != result_code::OK) //если не вышло
        {
            return std::move(make_tmpl_file());//возвращаем пустую либу
        }
        //если файл таки открылся, пытаемся его распарсить
        result<storage_data> parser_result = pm_parser->get_storage(get_file_res.m_object);
        
        if (parser_result.m_code != result_code::OK)//если не вышло
        {
            return std::move(make_tmpl_file());;//возвращаем пустую либу
        }
        
        return std::move(parser_result.m_object); //Если все ок, возвращаем результат
    }

    result_code store(std::function<result<std::string>(std::string&)> parser)
    {
    	result<std::string> res_get_file = this->get_string_from_file();
    	if (res_get_file.m_code != result_code::OK)
    	{
    		return res_get_file.m_code;
    	}

    	result<std::string> res_new_data = parser(res_get_file.m_object);

    	if (res_new_data.m_code != result_code::OK)
    	{
    		return res_new_data.m_code;
    	}

    	return this->set_data_to_file(res_new_data.m_object);
    }

    result_code add_book(std::shared_ptr<const Book> book)
    {   
    	std::function<result<std::string>(std::string&)> l_add_book = [this, book]
															   (std::string& str_file) -> result<std::string>
															   {return this->pm_parser->add_book(str_file, book);};

    	return this->store(l_add_book);
    }
    
    result_code add_author(std::shared_ptr<const Author> author)
    {
        std::function<result<std::string>(std::string&)> l_add_author = [this, author]
																 (std::string& str_file) -> result<std::string>
																 {return this->pm_parser->add_author(str_file, author);};

        return this->store(l_add_author);
    }
    
    result_code change_book(std::shared_ptr<const Book> book)
    {
        std::function<result<std::string>(std::string&)> l_change_book = [this, book]
																 (std::string& str_file) -> result<std::string>
																 {return this->pm_parser->change_book(str_file, book);};

        return this->store(l_change_book);
    }
    
    result_code change_author(std::shared_ptr<const Author> author)
    {
        std::function<result<std::string>(std::string&)> l_change_author = [this, author]
																  (std::string& str_file) -> result<std::string>
																  {return this->pm_parser->change_author(str_file, author);};

        return this->store(l_change_author);
    }
    
    result_code delete_book(int book_id)
    {
        std::function<result<std::string>(std::string&)> l_delete_book = [this, book_id]
																  (std::string& str_file) -> result<std::string>
																  {return this->pm_parser->delete_book(str_file, book_id);};

        return this->store(l_delete_book);
    }
    
    result_code delete_author(int author_id)
    {
        std::function<result<std::string>(std::string&)> l_delete_author = [this, author_id]
																	(std::string& str_file) -> result<std::string>
																	{return this->pm_parser->delete_author(str_file, author_id);};

        return this->store(l_delete_author);
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
    Library lib(std::unique_ptr<Storage>(new FileStorage(std::unique_ptr<Parser>(new JsonParser()), "FileStore.json")));
    //auto book = lib.get_book_by_id(1).m_object;
    //std::cout<<book->get_book_title()<<std::endl<<book->get_author()->get_name()<<std::endl;

    //auto add_author = lib.add_author(std::unique_ptr<Author>(new Author(1, "new author")));
    //auto author = lib.get_author_by_id(1).m_object;
    //auto add_book = lib.add_book(std::unique_ptr<Book>(new Book(1, "new book", author)));
    
    //auto change_book = lib.change_book(std::unique_ptr<Book>(new Book(5, "The new book", author)));
    //auto change_author = lib.change_author(std::unique_ptr<Author>(new Author(5, "The new author")));
    

    /*auto delete_book = lib.delete_book(10);
    if (delete_book == result_code::OK)
        std::cout<<"OK"<<std::endl;
    auto delete_author = lib.delete_author(3);
    if (delete_author == result_code::OK)
        std::cout<<"OK"<<std::endl;*/

}
