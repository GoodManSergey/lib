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
        //у shared_ptr есть cast к bool(), поэтому не нужно проверять != nullptr, есть более лаконичная запись
        return m_author ? m_author->get_author_id() : -1;
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


/*
 * -1,-2,-3 - это все magic-константы, какие-то непонятные числа. этого следует избегать. к тому же в разных методах они означают у тебя еще и разное!
 * Если тебе требуется возвращать результат в виде кода ошибки + какого-то дополнительного объекта, то есть рзаные способы:
 * 1) Возвращать всегда код выполнения (и это не magic-константа, а enum), а объект результат заполняется по ссылке
 *
 * result get_book(shared_ptr<const Book>& Book_ptr)
 *
 * 2) Или возвразщать результат в виде кода + объекта
 *
 * enum class result_code  : int {
 *    ok,
 *    book_not_found,
 *    author_not_found,
 *    invalid_input_params,
 *    ...
 * }
 *
 * если результат - толькод код , то :
 * resule_code delete_book(int id);
 *
 * если результата - не только код, то:
 *
 * template<typename T>
 * struct result {
 *     result_code m_code;
 *     T m_object;
 * }
 *
 * result<std::shared_ptr<const Book>> get_book(int id) const;
 *
 * 3) на худой конец, просто заменить magic-константы на enum (ниже я это делаю)
 *
 * Мы обычно используем способ 2, если проектируем какой-то синхронный API. Если же речь не идет о каком-то пубдичном API,
 *  то какой угодно способ.
 */

enum Error {
    invalid_input_params = -1,
    author_not_present = -2,
    internal_error = -3,
    author_not_found = -4,
};

class Library
{
    public:
    Library(Storage* storage): pm_storage(storage), m_next_author_id(1), m_next_book_id(1)
    {
    }
    
    //Возвращаем -1, если полученный указатель пуст
    //Возвращаем -3, если next_author_id оказался занят
    int add_author(std::unique_ptr<Author> author)
    {
        if (!author)
        {
            return invalid_input_params;
        }

        //TODO : странная проверка, логичнее проверять, что у автора id должен быть не задан
        if (m_author_list.find(author->get_author_id()) != m_author_list.end())
        {   
            std::cout<<"Something go wrong, next_author_id is busy"<<std::endl;
            return invalid_input_params;
        }
        
        author->set_author_id(m_next_author_id++);
        //TODO : посмотри что возвращает insert, вот тут как раз будет проверка при вставке, что такой id не был занят
        auto insert_result = m_author_list.insert({author->get_author_id(), std::move(author)});
        if (!insert_result.second) {
            std::cout<< "Something go wrong, next_author_id is busy" << std::endl;
            return internal_error;
        }
        return insert_result.first->first;
    }

    //Если автора нет, возвращаем пустой shared_ptr
    std::shared_ptr<const Author> get_author_by_id(int author_id)
    {
        //TODO : дважды делается поиск, сперва в find, поток в operator []
        //если ты уже нашел и find вернул итератор - так верни объект :
        // return it->second;
        if (m_author_list.find(author_id) != m_author_list.end())
        {   
            return m_author_list[author_id];
        }
        //TODO : чтобы вернуть пустой объект можно сразу написать его конструктор в return, незачем объявляеть его
        return {};
    }

    //Если автора не уществует, то вернется пустой лист и переменнная have_list будет установлена в false
    std::vector<std::shared_ptr<const Book>> get_authors_books(int author_id, bool& have_list)
    {
        //TODO : оже самое, поиск идет дважды
        if (m_authors_books.find(author_id) == m_authors_books.end())
        {   
            have_list = false;
            return {};
        }
        
        have_list = true;
        return m_authors_books[author_id];
    }
    
    //Возвращаем -1, если получили пустой указатель
    //-2, если такого автора нет
    //Если такая книга уже есть -3
    int add_book(std::unique_ptr<Book> book)
    {    
        if (!book)
        {
            return invalid_input_params;
        }
        
        int author_id = book->get_author_id();
        
        if (author_id < 1)
        {
            return author_not_present;
        }

        //TODO : дважды поиск автора (find / operator [] ниже)
        if (m_author_list.find(author_id) == m_author_list.end())
        {   
            return -2;
        }
        
        std::shared_ptr<Author> author = m_author_list[author_id];
        
        book->set_author(author);
        
        int book_id = m_next_book_id;

        //TODO : опять странная проверка, лучше проверить что id не задан
        if (m_book_list.find(book_id) != m_book_list.end())
        {   
            std::cout<<"Something go wrong, next_book_id is busy"<<std::endl;
            return -3;
        }
        
        book->set_book_id(book_id);
        m_next_book_id++;

        //TODO : а вот тут проверка, что в хранилище id не задан можно сделать через insert, как в примере с автором выше
        m_book_list[book_id] = std::move(book);
        
        m_authors_books[author_id].push_back(m_book_list[book_id]);

        return book_id;    
    }
    
    //Если книги не существует возвращаем пустой указатель 
    std::shared_ptr<const Book> get_book_by_id(int book_id)
    {
        //TODO : дважды поиск
        if (m_book_list.find(book_id) != m_book_list.end())
        {   
            return m_book_list[book_id];
        }
        return {};
    }
    
    //Если автора с таким id нет, то возвращаем -2
    int change_author(std::unique_ptr<Author> author)
    {
        //TODO : опять двойной поиск find / ниже operator []
        if (m_author_list.find(author->get_author_id()) == m_author_list.end())
        {
            return author_not_found;
        }
        
        std::vector<std::shared_ptr<const Book>> new_authors_books;

        //TODO : каждый раз обращаясь по operator [] в map ты делаешь поиск, это не очень хорошо

        int auth_id = author->get_author_id();
        m_author_list[auth_id] = std::move(author);

        /*
        for (auto && book_ptr : m_authors_books[auth_id]) {
            //конструктор копирования, сколько бы не добавлялось потом полей все будет работать и не придется переписывать код
            auto new_book_ptr = std::make_shared<Book>(*book_ptr);
            new_book_ptr->set_author(m_author_list[auth_id]);
            new_authors_books.push_back(new_book_ptr);
            m_book_list[book_ptr->get_book_id()] = new_book_ptr;
        }
         */

        //TODO : можно переписать цикл проще и универсальнее (пример выше)
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
        //TODO : не смотрел, перепиши с учетом всех замечаний выше
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
    Storage* pm_storage;
    
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
    std::cout<<gotten_book_2->get_book_title()<<std::endl;


    return 0;
}
