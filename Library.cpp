#include<memory>
#include<string>
#include<iostream>
#include<map>
#include<vector>
#include <algorithm>



class Author
{
    public:
    Author(int id, std::string name): author_id(id), name(name)
    {}
    
    int get_author_id() const {return author_id;}
    std::string get_name() const {return name;}

    void set_author_id(int id)
    {
        this->author_id = id;
    }

    void set_name(std::string name)
    {
        this->name = name;
    }

    private:
    std::string name;
    int author_id;

};


class Book
{   
    public:
    Book(int book_id, std::string title, std::shared_ptr<const Author> author): 
        book_id(book_id), title(title), author(author)
    {}        
    
    int get_book_id() const {return book_id;}
    std::string get_book_title() const {return title;}
    std::shared_ptr<const Author> get_author() const {return author;}

    int get_author_id() const {return this->author->get_author_id();}
    std::string get_author_name() const {return this->author->get_name();}

    void set_book_id(int new_book_id)
    {
        this->book_id = new_book_id;
    }

    void set_title(std::string new_title)
    {
        this->title = new_title;
    }

    void set_author(const std::shared_ptr<Author> new_author)
    {
        this->author = new_author;
    }

    private:
    int book_id;
    std::string title;
    std::shared_ptr<const Author> author;    
};


class Storage
{
    public:
    virtual get_books() = 0;
    virtual get_authors() = 0;
    virtual add_book() = 0;
    virtual add_author() = 0;
    virtual change_book() = 0;
    virtual change_author() = 0;
    virtual delete_book() = 0;
    virtual delete_author() = 0;
};


class Library
{
    public:
    Library(Storage* storage): storage(storage), next_author_id(1), next_book_id(1)
    {
    }
    
    int add_author(const Author& author)
    {
        std::shared_ptr<Author> new_author(new Author(next_author_id, author.get_name()));
        next_author_id++;

        author_list.insert(std::pair<int, std::shared_ptr<Author>>(new_author->get_author_id(), new_author));
        //Как тебе такое, Илон Маск?
        //authors_books.insert(std::pair<int, std::shared_ptr<std::vector<std::shared_ptr<const Book>>>>(new_author->get_author_id(), std::shared_ptr<std::vector<std::shared_ptr<const Book>>>(new std::vector<std::shared_ptr<const Book>>)));
        return new_author->get_author_id();
    }

    std::shared_ptr<const Author> get_author_by_id(const int& author_id)
    {   
        return author_list.at(author_id);
    }

    std::vector<std::shared_ptr<const Book>> get_authors_books(const int& author_id)
    {
        return authors_books.at(author_id);
    }

    int add_book(const Book& book)
    {   
        std::shared_ptr<Author> author = author_list.at(book.get_author_id());
        std::shared_ptr<Book> new_book(new Book(next_book_id, book.get_book_title(), author));
        next_book_id++;

        book_list.insert(std::pair<int, std::shared_ptr<Book>>(new_book->get_book_id(), new_book));
        
        authors_books[new_book->get_author_id()].push_back(new_book);

        return new_book->get_book_id();
           
    }

    std::shared_ptr<const Book> get_book_by_id(const int& book_id)
    {
        return book_list[book_id];
    }

    int change_author(const Author& author)
    {
        std::vector<std::shared_ptr<const Book>> new_authors_books;
        std::shared_ptr<Author> new_author(new Author(author.get_author_id(), author.get_name()));
        
        for (auto iter = authors_books.at(new_author->get_author_id()).begin(); iter != authors_books.at(new_author->get_author_id()).end(); iter++)
        {
            std::shared_ptr<Book> new_book(new Book((*iter)->get_book_id(), (*iter)->get_book_title(), new_author));
            new_authors_books.push_back(new_book);
            book_list[(*iter)->get_book_id()] = new_book;
        }
        
        author_list[new_author->get_author_id()] = new_author;
        authors_books[new_author->get_author_id()] = new_authors_books;

        return new_author->get_author_id();
    }

    int change_book(const Book& book)
    {	
	    std::shared_ptr<Author> author = author_list.at(book.get_author_id());
	    std::shared_ptr<Book> old_book = book_list.at(book.get_book_id());
	    std::shared_ptr<Book> new_book(new Book(book.get_book_id(), book.get_book_title(), author));
        
        book_list[book.get_book_id()] = new_book;
        
        std::vector<std::shared_ptr<const Book>> & author_books = authors_books.at(book.get_book_id());
        
        author_books.erase(std::remove(author_books.begin(), author_books.end(), old_book), author_books.end());
        
        return new_book->get_book_id();
    }

    private:
    int next_author_id;
    int next_book_id;
    std::map<int, std::shared_ptr<Author>> author_list;
    std::map<int, std::shared_ptr<Book>> book_list;
    //std::map<int, std::shared_ptr<std::vector<std::shared_ptr<const Book>>>> authors_books;
    std::map<int, std::vector<std::shared_ptr<const Book>>> authors_books;
    Storage* storage;
    
};


int main()
{   
    Storage* store = new Storage();
    Library lib(store);
    
    Author author(1, "Oruell");

    //Добавляем автора
    int auth_id = lib.add_author(author);
    
    //Получаем автора
    std::shared_ptr<const Author> author_2 = lib.get_author_by_id(auth_id);

    Book book = Book(1, "1984", author_2);
    //Добавляем книгу
    int book_id = lib.add_book(book);

    author.set_name("Ne Pushkin");
    
    //Получаем книгу
    std::shared_ptr<const Book> gotten_book= lib.get_book_by_id(book_id);
    
    //Изменяем автора
    lib.change_author(author);

    //Изменяеи книгу(название)
    book.set_title("ne ev on");
    lib.change_book(book);

    //Получаем ту же книгу
    std::shared_ptr<const Book> gotten_book_2 = lib.get_book_by_id(book_id);
    
    
    
    std::cout<<gotten_book->get_book_id()<<std::endl;
    std::cout<<gotten_book->get_author_name()<<std::endl;
    std::cout<<gotten_book->get_book_title()<<std::endl;
    std::cout<<"-------------------"<<std::endl;
    std::cout<<gotten_book_2->get_book_id()<<std::endl;
    std::cout<<gotten_book_2->get_author_name()<<std::endl;
    std::cout<<gotten_book_2->get_book_title()<<std::endl;


    return 0;
}
