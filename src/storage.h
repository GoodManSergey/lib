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
    virtual ~Storage() = default;
};
