#include"file_storage.h"


FileStorage::FileStorage(std::unique_ptr<Parser> parser, const std::string& storage_path):
        pm_parser(std::move(parser)),
        m_storage_path(storage_path)
    {};

result<std::string> FileStorage::get_string_from_file()
    {
        std::ifstream file(m_storage_path);

        if (file.is_open())
        {
            std::string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
            return std::move(str);
        }
        else
        {
            std::ifstream file_tmp(m_storage_path + ".tmp");

            if (!file_tmp.is_open())
            {
                return result_code::OPEN_FILE_ERROR;
            }

            std::string str((std::istreambuf_iterator<char>(file_tmp)),
                 std::istreambuf_iterator<char>());

            bool rename_status = std::rename((m_storage_path + ".tmp").c_str(), m_storage_path.c_str());

            assert(!rename_status);

            return std::move(str);
        }
    };

result_code FileStorage::make_file(const std::string& str_data, const std::string& postfix="")
    {
        std::ofstream file(m_storage_path + postfix);

       if (!file.is_open())
       {
    	   return result_code::OPEN_FILE_ERROR;
       }

        file << str_data;
        file.close();

        return result_code::OK;
    };

result_code FileStorage::set_data_to_file(const std::string& str_data)
    {
        std::string tmp_postfix = ".tmp";

        result_code make_file_res = make_file(str_data, tmp_postfix);

        if (make_file_res != result_code::OK)
        {
        	return make_file_res;
        }

        bool rename_status = std::rename((m_storage_path + tmp_postfix).c_str(), m_storage_path.c_str());

        assert(!rename_status);

        return result_code::OK;
    };

storage_data FileStorage::make_tmpl_file()
    {
    	std::string file_tmpl = pm_parser->set_empty_tmpl(); //получаем паустой шаблон и заполняем его в парсер
    	result_code tmpl_file_res = make_file(file_tmpl); //записываем его в файл, без tmp, т.к. файл уже битый
    	assert(tmpl_file_res == result_code::OK);//если не вышло записать, то бросаем ошибку
    	storage_data empty_data {1,1, {}, {}};
    	return std::move(empty_data);//возвращаем пустую либу
    };

storage_data FileStorage::get_storage()
    {
        result<std::string> get_file_res = get_string_from_file(); //пытаемся получить файл

        if (get_file_res.m_code != result_code::OK) //если не вышло
        {
            std::cout<<"can not open file while init"<<std::endl;
            return std::move(make_tmpl_file());//возвращаем пустую либу
        }
        //если файл таки открылся, пытаемся его распарсить
        result<storage_data> parser_result = pm_parser->get_storage(get_file_res.m_object);

        if (parser_result.m_code != result_code::OK)//если не вышло
        {
            std::cout<<"can not parse file while init"<<std::endl;
            return std::move(make_tmpl_file());;//возвращаем пустую либу
        }

        return std::move(parser_result.m_object); //Если все ок, возвращаем результат
    };

result_code FileStorage::store(const std::function<result<std::string>()>& parser)
    {
    	result<std::string> res_new_data = parser();

    	if (res_new_data.m_code != result_code::OK)
    	{
    		return res_new_data.m_code;
    	}

    	return set_data_to_file(res_new_data.m_object);
    };

result_code FileStorage::add_book(std::shared_ptr<const Book> book)
    {
    	std::function<result<std::string>()> l_add_book = [this, book]
															   () -> result<std::string>
															   {return pm_parser->add_book(book);};

    	return store(l_add_book);
    };

result_code FileStorage::add_author(std::shared_ptr<const Author> author)
    {
        std::function<result<std::string>()> l_add_author = [this, author]
																 () -> result<std::string>
																 {return pm_parser->add_author(author);};

        return store(l_add_author);
    };

result_code FileStorage::change_book(std::shared_ptr<const Book> book)
    {
        std::function<result<std::string>()> l_change_book = [this, book]
																 () -> result<std::string>
																 {return pm_parser->change_book(book);};

        return store(l_change_book);
    };

result_code FileStorage::change_author(std::shared_ptr<const Author> author)
    {
        std::function<result<std::string>()> l_change_author = [this, author]
																  () -> result<std::string>
																  {return pm_parser->change_author(author);};

        return store(l_change_author);
    };

result_code FileStorage::delete_book(int book_id)
    {
        std::function<result<std::string>()> l_delete_book = [this, book_id]
																  () -> result<std::string>
																  {return pm_parser->delete_book(book_id);};

        return store(l_delete_book);
    };

result_code FileStorage::delete_author(int author_id)
    {
        std::function<result<std::string>()> l_delete_author = [this, author_id]
																	() -> result<std::string>
																	{return pm_parser->delete_author(author_id);};

        return store(l_delete_author);
    };
