class XmlParser: public Parser
{
    public:
    std::string set_empty_tmpl()
    {
        pugi::xml_node root = m_doc.append_child("root");
        root.append_child("books");
        root.append_child("authors");

        pugi::xml_node next_book_id = root.append_child("next_book_id");
        pugi::xml_node next_author_id = root.append_child("next_author_id");

        next_book_id.append_attribute("id") = 1;
        next_author_id.append_attribute("id") = 1;

        return std::move(to_string());
    }

    std::string to_string()
    {
        std::stringstream buffer;
    	m_doc.save(buffer);

    	return std::move(buffer.str());
    }

    result<storage_data> get_storage(const std::string& file_str)
    {
        if (!m_doc.load_string(file_str.c_str()))
        {
        	return result_code::PARSER_ERROR;
        }

        pugi::xml_node root = m_doc.document_element();

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
        pugi::xml_node root = m_doc.document_element();

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

        return std::move(to_string());
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
        return std::move(add_node(file_str, books_query, next_book_id_path, book_node_filler));
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
        return std::move(add_node(file_str, authors_query, next_author_id_path, author_node_filler));
    }

    result<std::string> change_node(const std::string& file_str, const pugi::xpath_query& nodes_path,
                                     std::function<void(pugi::xml_node)> node_filler, std::function<bool(pugi::xml_node)> node_finder)
    {
        pugi::xml_node root = m_doc.document_element();

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

        return std::move(to_string());
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

        return std::move(change_node(file_str, books_query, book_node_filler, find_book_by_id));
    }

    result<std::string> change_author(const std::string& file_str, std::shared_ptr<const Author> author)
    {
        pugi::xpath_query authors_query("/data/authors");
    	auto find_author_by_id = [author_id = author->get_author_id()] (pugi::xml_node node) -> bool {return node.attribute("id").as_int() == author_id; };

    	auto author_node_filler = [author](pugi::xml_node author_node)-> void
    	                                {
    	                                    author_node.attribute("name").set_value(author->get_name().c_str());
    	                                };

        return std::move(change_node(file_str, authors_query, author_node_filler, find_author_by_id));
    }

    result<std::string> delete_node_by_id(const std::string& file_str, const pugi::xpath_query& nodes_path, int node_id)
    {
        pugi::xml_node root = m_doc.document_element();

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

        return std::move(to_string());
    }

    result<std::string> delete_book(const std::string& file_str, int book_id)
    {
    	pugi::xpath_query books_query("/data/books");

    	return std::move(delete_node_by_id(file_str, books_query, book_id));
    }

    result<std::string> delete_author(const std::string& file_str, int author_id)
    {
    	pugi::xpath_query authors_query("/data/authors");

    	return std::move(delete_node_by_id(file_str, authors_query, author_id));
    }

    private:
        pugi::xml_document m_doc;
};
