#include"pugixml.hpp"
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<assert.h>


class XmlParser
{
    public:
    std::string get_storage(const std::string& file_str)
    {
        pugi::xml_document doc;
        
        assert(doc.load_string(file_str.c_str()));
        pugi::xml_node root = doc.document_element();
        
        pugi::xpath_query books_query("/data/books/book");
        pugi::xpath_query authors_query("/data/authors/author");
        pugi::xpath_query next_book_id_path("/data/next_book_id");
        pugi::xpath_query next_author_id_path("/data/next_author_id");
        
        for (pugi::xpath_node book_query : root.select_nodes(books_query))
        {
            pugi::xml_node book = book_query.node();
            std::cout<<book.attribute("title").value()<<std::endl;
            std::cout<<book.attribute("id").as_int()<<std::endl;
            std::cout<<book.attribute("author_id").as_int()<<std::endl;
        }
        
        
        std::stringstream ss;
        doc.save(ss,"  ");
        std::cout<<ss.str()<<std::endl;
        std::cout<<root.select_single_node(next_book_id_path).node().attribute("id").as_int()<<std::endl;
        
        
        
        return "";
        
    }
};

int main()
{
    XmlParser par;
    
    std::ifstream file("FileStore.xml");
        std::stringstream buffer; 
        
        if (file.is_open())
        {
            buffer << file.rdbuf();
            file.close();
        }
                
    std::cout<<par.get_storage(buffer.str());
    return 0;
}
