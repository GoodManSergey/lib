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

    int add_book(const Book& book)
    {   //Принимать указатель шаред птр и возвращать -1, если он пустой
        //Проверять, что автор существует, не получать его с помощью at и если его нет, то возвращать -2
        std::shared_ptr<Author> author = m_author_list.at(book.get_author_id());
        std::shared_ptr<Book> new_book(new Book(m_next_book_id, book.get_book_title(), author));
        //проверять что книги с next book id еще нет, если есть, то возвращаем -3
        m_next_book_id++;
        
        //убрать инсерт
        m_book_list.insert(std::pair<int, std::shared_ptr<Book>>(new_book->get_book_id(), new_book));
        
        m_authors_books[new_book->get_author_id()].push_back(new_book);

        return new_book->get_book_id();
           
    }

    std::shared_ptr<const Book> get_book_by_id(int book_id)
    {   //проверять, что такая книга существует, если нет, то возвращаем пустой shared_ptr
        return m_book_list[book_id];
    }

    int change_author(const Author& author)
    {//принимать shared_ptr
     //Првоерять, что автор с таким id существует, если нет , то возвращаем -2
        std::vector<std::shared_ptr<const Book>> new_authors_books;
        std::shared_ptr<Author> new_author(new Author(author.get_author_id(), author.get_name()));
        //убрать at
        for (auto iter = m_authors_books.at(new_author->get_author_id()).begin(); iter != m_authors_books.at(new_author->get_author_id()).end(); iter++)
        {
            std::shared_ptr<Book> new_book(new Book((*iter)->get_book_id(), (*iter)->get_book_title(), new_author));
            new_authors_books.push_back(new_book);
            m_book_list[(*iter)->get_book_id()] = new_book;
        }
        
        m_author_list[new_author->get_author_id()] = new_author;
        m_authors_books[new_author->get_author_id()] = new_authors_books;

        return new_author->get_author_id();
    }

    int change_book(const Book& book)
    {	//принимать shared_ptr
        //Проверять, что книга с таким id существует, если нет, то возвращать -4
        //Проверить, что автор с таким id существует, если нет, то возвращать -2
        //Убрать at
	    std::shared_ptr<Author> author = m_author_list.at(book.get_author_id());
	    //убрать at добавить проверку
	    std::shared_ptr<Book> old_book = m_book_list.at(book.get_book_id());
	    std::shared_ptr<Book> new_book(new Book(book.get_book_id(), book.get_book_title(), author));
        
        m_book_list[book.get_book_id()] = new_book;
        
        //убрать at
        std::vector<std::shared_ptr<const Book>>& author_books = m_authors_books.at(book.get_book_id());
        
        author_books.erase(std::remove(author_books.begin(), author_books.end(), old_book), author_books.end());
        
        return new_book->get_book_id();
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

    Book book = Book(1, "1984", author_2);
    //Добавляем книгу
    int book_id = lib.add_book(book);
    
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
    
    Author author_3(1, "Ne Pushkin");
    //Изменяем автора
    lib.change_author(author_3);

    //Изменяеи книгу(название)
    book.set_title("ne ev on");
    lib.change_book(book);

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
