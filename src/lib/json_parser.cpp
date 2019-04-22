#include"json_parser.h"


std::string JsonParser::set_empty_tmpl()
    {
        m_root["authors"] = Json::arrayValue;
        m_root["books"] = Json::arrayValue;
        m_root["next_author_id"] = 1;
		m_root["next_book_id"] = 1;

        return std::move(to_string());
    };

std::string JsonParser::to_string()
    {
        Json::FastWriter fastWriter;
        std::string output = fastWriter.write(m_root);

        return std::move(output);
    };

result<storage_data> JsonParser::get_storage(const std::string& file_str)
    {
        Json::Reader reader;

        if (!reader.parse(file_str.c_str(), m_root ))
        {
            std::cout<<"parser error"<<std::endl;
            return result_code::PARSER_ERROR;
        }

        std::vector<std::shared_ptr<Author>> author_list;
        std::vector<std::shared_ptr<Book>> book_list;

        if (!m_root.isMember("authors"))
        {
            std::cout<<"no authors error"<<std::endl;
            return result_code::PARSER_ERROR;
        }

        auto authors = m_root["authors"];

        if (!authors.isArray())
        {
            return result_code::PARSER_ERROR;
        }

        for (auto author : authors)
        {
            if (!author.isObject())
            {
                continue;
            }

            if (!(author.isMember("id") and author.isMember("name")))
            {
                continue;
            }

            auto author_id = author["id"];
            auto author_name = author["name"];

            if (!(author_id.isInt() and author_name.isString()))
            {
                continue;
            }

            int author_id_int = author_id.asInt();
            std::string author_name_str = author_name.asString();

            author_list.push_back(std::shared_ptr<Author>(new Author(author_id_int, author_name_str)));
        }

        if (!m_root.isMember("books"))
        {
            std::cout<<"no books error"<<std::endl;
            return result_code::PARSER_ERROR;
        }

        auto books = m_root["books"];

        if (!books.isArray())
        {
            return result_code::PARSER_ERROR;
        }

        for (auto book : books)
        {
            if (!book.isObject())
            {
                continue;
            }

            if (!(book.isMember("id") and book.isMember("title") and book.isMember("author_id")))
            {
                continue;
            }

            auto book_id = book["id"];
            auto book_author_id = book["author_id"];
            auto book_title = book["title"];

            if (!(book_id.isInt() and book_author_id.isInt() and book_title.isString()))
            {
                continue;
            }

            int book_id_int = book_id.asInt();
            int book_author_id_int = book_author_id.asInt();
            std::string book_title_str = book_title.asString();

            auto author_iter = std::find_if(author_list.begin(), author_list.end(),
             [book_author_id_int] (std::shared_ptr<Author>  author) -> bool { return author->get_author_id() == book_author_id_int; });

            if (author_iter==author_list.end())
            {
                continue;
            }

            book_list.push_back(std::shared_ptr<Book>(new Book(book_id_int, book_title_str, (*author_iter))));
        }

        if (!m_root.isMember("next_book_id"))
        {
            return result_code::PARSER_ERROR;
        }

        if (!m_root.isMember("next_author_id"))
        {
            return result_code::PARSER_ERROR;
        }

        auto next_book_id = m_root["next_book_id"];
        auto next_author_id = m_root["next_author_id"];

        if (!next_book_id.isInt())
        {
            return result_code::PARSER_ERROR;
        }

        if (!next_author_id.isInt())
        {
            return result_code::PARSER_ERROR;
        }

        int next_book_id_int = next_book_id.asInt();
        int next_author_id_int = next_author_id.asInt();


        storage_data data {next_book_id_int, next_author_id_int, author_list, book_list};
        return std::move(data);
    };

result<std::string> JsonParser::add_node(const std::string& node_name, const std::string& node_next_id, std::function<int(Json::Value&)> node_filler)
    {
        if (!m_root.isMember(node_name))
        {
            return result_code::PARSER_ERROR;
        }

        if (!m_root[node_name].isArray())
        {
            return result_code::PARSER_ERROR;
        }

        if (!m_root.isMember(node_next_id))
        {
            return result_code::PARSER_ERROR;
        }

        Json::Value node;
        int id = node_filler(node);

        try
        {
            m_root[node_name].append(node);
        }
        catch(Json::Exception const&)
        {
            return result_code::PARSER_ERROR;
        }

        m_root[node_next_id] = id + 1;

        return std::move(to_string());
    };

result<std::string> JsonParser::add_book(std::shared_ptr<const Book> book)
    {
        auto book_filler = [book](Json::Value& node) -> int
        {
            node["id"] = book->get_book_id();
            node["title"] = book->get_book_title();
            node["author_id"] = book->get_author_id();
            return book->get_book_id();
        };

        return std::move(add_node("books", "next_book_id", book_filler));
    };

result<std::string> JsonParser::add_author(std::shared_ptr<const Author> author)
    {
        auto author_filler = [author](Json::Value& node) -> int
        {
            node["id"] = author->get_author_id();
            node["name"] = author->get_name();
            return author->get_author_id();
        };

        return std::move(add_node("authors", "next_author_id", author_filler));
    };

result<std::string> JsonParser::change_node_by_id(const std::string& node_name, int id, std::function<void(Json::Value&)> node_changer)
    {
        if (!m_root.isMember(node_name))
        {
            return result_code::PARSER_ERROR;
        }

        if (!m_root[node_name].isArray())
        {
            return result_code::PARSER_ERROR;
        }

        bool changed = false;
        for (auto& node : m_root[node_name])
        {
            if (!node.isMember("id"))
            {
                continue;
            }

            auto node_id = node["id"];

            if (!node_id.isInt())
            {
                continue;
            }

            if (node_id.asInt() == id)
            {
                node_changer(node);
                changed = true;
                break;
            }
        }

        if (!changed)
        {
            return result_code::PARSER_ERROR;
        }

        return std::move(to_string());
    };

result<std::string> JsonParser::change_book(std::shared_ptr<const Book> book)
    {
        auto book_changer = [book](Json::Value& node) -> void
                            {
                                node["title"] = book->get_book_title();
                                node["author_id"] = book->get_author_id();
                            };

        return std::move(change_node_by_id("books", book->get_book_id(), book_changer));
    };

result<std::string> JsonParser::change_author(std::shared_ptr<const Author> author)
    {
        auto author_changer = [author](Json::Value& node) -> void
                            {
                                node["name"] = author->get_name();
                            };

        return std::move(change_node_by_id("authors", author->get_author_id(), author_changer));
    };

result<std::string> JsonParser::delete_node_by_id(int id, const std::string& node_name)
    {
        if (!m_root.isMember(node_name))
        {
            return result_code::PARSER_ERROR;
        }

        if (!m_root[node_name].isArray())
        {
            return result_code::PARSER_ERROR;
        }

        bool changed = false;

        Json::Value new_nodes;

        for (auto&& node : m_root[node_name])
        {
            if (!node.isMember("id"))
            {
                continue;
            }

            auto node_id = node["id"];

            if (!node_id.isInt())
            {
                continue;
            }

            if (node_id.asInt() == id)
            {
                continue;
            }

            new_nodes.append(node);
        }

        if (!changed)
        {
            return result_code::PARSER_ERROR;
        }

        m_root[node_name] = std::move(new_nodes);

        return std::move(to_string());
    };

result<std::string> JsonParser::delete_book(int book_id)
    {
        return std::move(delete_node_by_id(book_id, "books"));
    };

result<std::string> JsonParser::delete_author(int author_id)
    {
        return std::move(delete_node_by_id(author_id, "authors"));
    };
