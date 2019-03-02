#include<memory>
#include<string>
#include<iostream>
#include<unordered_map>
#include<vector>
#include<algorithm>



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
        if (this->m_author != nullptr)
        {
            return this->m_author->get_author_id();
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


class Storage
{
   /* public:
    virtual get_books() = 0;
    virtual get_authors() = 0;
    virtual add_book() = 0;
    virtual add_author() = 0;
    virtual change_book() = 0;
    virtual change_author() = 0;
    virtual delete_book() = 0;
    virtual delete_author() = 0;*/
};


class Library
{
    public:
    Library(Storage* storage): m_storage(storage), m_next_author_id(1), m_next_book_id(1)
    {
    }
    
    //Возвращаем -1, если полученный указатель пуст
    //Возвращаем -3, если next_author_id оказался занят
    int add_author(std::unique_ptr<Author> author)
    {
        if (author == nullptr)
        {
            return -1;
        }
        
        if (m_author_list.find(author->get_author_id()) != m_author_list.end())
        {   
            std::cout<<"Something go wrong, next_author_id is busy"<<std::endl;
            return -3;
        }
        
        author->set_author_id(m_next_author_id);

        
        m_next_author_id++;
        
        int author_id = author->get_author_id();

        m_author_list[author_id] = std::move(author);
        
        return author_id;
    }

    //Если автора нет, возвращаем пустой shared_ptr
    std::shared_ptr<const Author> get_author_by_id(int author_id)
    { 
        if (m_author_list.find(author_id) != m_author_list.end())
        {   
            return m_author_list[author_id];
        }
        
        std::shared_ptr<Author> none_author;
        
        return none_author;
    }

    //Если автора не уществует, то вернется пустой лист и переменнная have_list будет установлена в false
    std::vector<std::shared_ptr<const Book>> get_authors_books(int author_id, bool& have_list)
    {
        if (m_authors_books.find(author_id) == m_authors_books.end())
        {   
            have_list = false;
            std::vector<std::shared_ptr<const Book>> empty_vector;
            return empty_vector;
        }
        
        have_list = true;
        return m_authors_books[author_id];
    }
    
    //Возвращаем -1, если получили пустой указатель
    //-2, если такого автора нет
    //Если такая книга уже есть -3
    int add_book(std::unique_ptr<Book> book)
    {    
        if (book == nullptr)
        {
            return -1;
        }
        
        int author_id = book->get_author_id();
        
        if (author_id < 1)
        {
            return -2;
        }
        
        if (m_author_list.find(author_id) == m_author_list.end())
        {   
            return -2;
        }
        
        std::shared_ptr<Author> author = m_author_list[author_id];
        
        book->set_author(author);
        
        int book_id = m_next_book_id;
        
        if (m_book_list.find(book_id) != m_book_list.end())
        {   
            std::cout<<"Something go wrong, next_book_id is busy"<<std::endl;
            return -3;
        }
        
        book->set_book_id(book_id);
        m_next_book_id++;
        
        m_book_list[book_id] = std::move(book);
        
        m_authors_books[author_id].push_back(m_book_list[book_id]);

        return book_id;    
    }
    
    //Если книги не существует возвращаем пустой указатель 
    std::shared_ptr<const Book> get_book_by_id(int book_id)
    {
        if (m_book_list.find(book_id) != m_book_list.end())
        {   
            return m_book_list[book_id];
        }
        
        std::shared_ptr<Book> none_book;
        
        return none_book;
    }
    
    //Если автора с таким id нет, то возвращаем -2
    int change_author(std::unique_ptr<Author> author)
    {
        if (m_author_list.find(author->get_author_id()) == m_author_list.end())
        {
            return -2;
        }
        
        std::vector<std::shared_ptr<const Book>> new_authors_books;
        
        int auth_id = author->get_author_id();
        m_author_list[auth_id] = std::move(author);

        for (auto iter = m_authors_books[auth_id].begin(); iter != m_authors_books[auth_id].end(); iter++)
        {
            std::shared_ptr<Book> new_book(new Book((*iter)->get_book_id(), (*iter)->get_book_title(), m_author_list[auth_id]));
            new_authors_books.push_back(new_book);
            m_book_list[(*iter)->get_book_id()] = new_book;
        }
        
        m_authors_books[auth_id] = new_authors_books;

        return auth_id;
    }

    //Если автора нет, то -2
    //Если нет книги с таким id, то -4
    int change_book(std::unique_ptr<Book> book)
    {	
        if (m_author_list.find(book->get_author_id()) == m_author_list.end())
        {
            return -2;
        }
        
        if (m_book_list.find(book->get_book_id()) == m_book_list.end())
        {
            return -4;
        }
        
	    book->set_author(m_author_list[book->get_author_id()]);
        
        int book_id = book->get_book_id();
	    std::shared_ptr<Book> old_book = m_book_list[book_id];
        
        m_book_list[book_id] = std::move(book);
        
        std::vector<std::shared_ptr<const Book>>& author_books = m_authors_books[book_id];
        
        author_books.erase(std::remove(author_books.begin(), author_books.end(), old_book), author_books.end());
        
        author_books.push_back(m_book_list[book_id]);
        
        return book_id;
    }

    private:
    int m_next_author_id;
    int m_next_book_id;
    std::unordered_map<int, std::shared_ptr<Author>> m_author_list;
    std::unordered_map<int, std::shared_ptr<Book>> m_book_list;
    std::unordered_map<int, std::vector<std::shared_ptr<const Book>>> m_authors_books;
    Storage* m_storage;
    
};


int main()
{   
    Storage* store = new Storage();
    Library lib(store);
    
    std::unique_ptr<Author> author(new Author(1, "Oruell"));

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
    //gotten_book->set_title("ne ev on");
    //lib.change_book(book);

    //Получаем ту же книгу
    std::shared_ptr<const Book> gotten_book_2 = lib.get_book_by_id(book_id);
    
    
    
    std::cout<<gotten_book->get_book_id()<<std::endl;
    std::cout<<gotten_book->get_author()->get_name()<<std::endl;
    std::cout<<gotten_book->get_book_title()<<std::endl;
    std::cout<<"-------------------"<<std::endl;
    std::cout<<gotten_book_2->get_book_id()<<std::endl;
    std::cout<<gotten_book_2->get_author()->get_name()<<std::endl;
    std::cout<<gotten_book_2->get_book_title()<<std::endl;


    return 0;
}
