#include"server.h"


Server::Server(std::unique_ptr<Library> lib, std::unique_ptr<Socket> server_socket):
        pm_lib(std::move(lib)), m_work(true), pm_server_socket(std::move(server_socket))
    {
        m_commands["add_author"] = server_command::ADD_AUTHOR;
        m_commands["get_author_by_id"] = server_command::GET_AUTHOR_BY_ID;
        m_commands["change_author"] = server_command::CHANGE_AUTHOR;
        m_commands["delete_author_by_id"] = server_command::DELETE_AUTHOR_BY_ID;
        m_commands["add_book"] = server_command::ADD_BOOK;
        m_commands["get_book"] = server_command::GET_BOOK;
        m_commands["change_book"] = server_command::CHANGE_BOOK;
        m_commands["delete_book"] = server_command::DELETE_BOOK;
        m_commands["get_all_author_books"] = server_command::GET_ALL_AUTHOR_BOOKS;
    };

std::string Server::json_to_string(const Json::Value& json)
	{
		Json::FastWriter fastWriter;
        std::string output = fastWriter.write(json);

        return std::move(output);
	};

std::string Server::add_author(const Json::Value& data)
	{
		if (!data.isMember("name"))
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"no name field"<<std::endl;
			return std::move(error_return(error));
		}

		auto author_name_json = data["name"];

		if (!author_name_json.isString())
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"name is not string"<<std::endl;
			return std::move(error_return(error));
		}


		result<int> res = pm_lib->add_author(std::unique_ptr<Author>(new Author(1, author_name_json.asString())));

		if (res.m_code == result_code::OK)
		{
			Json::Value data;
			data["id"] = res.m_object;

			return std::move(make_resp(data, ServerStatus::ok()));
		}
		else
		{
			std::cout<<"lib error"<<std::endl;
			return std::move(error_return(ServerStatus::lib_error()));
		}
	};

std::string Server::get_author(const Json::Value& data)
	{
		if (!data.isMember("id"))
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"no name field"<<std::endl;
			return std::move(error_return(error));
		}

		auto author_id_json = data["id"];

		if (!author_id_json.isInt())
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"name is not string"<<std::endl;
			return std::move(error_return(error));
		}

		auto res = pm_lib->get_author_by_id(author_id_json.asInt());

        if (res.m_code == result_code::OK)
		{
			Json::Value data;
			data["id"] = res.m_object->get_author_id();
			data["name"] = res.m_object->get_name();

			return std::move(make_resp(data, ServerStatus::ok()));
		}
		else
		{
			std::cout<<"lib error"<<std::endl;
			return std::move(error_return(ServerStatus::lib_error()));
		}
	};

std::string Server::change_author(const Json::Value& data)
	{
		if (!data.isMember("name"))
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"no name field"<<std::endl;
			return std::move(error_return(error));
		}

		auto author_name_json = data["name"];

		if (!author_name_json.isString())
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"name is not string"<<std::endl;
			return std::move(error_return(error));
		}

		if (!data.isMember("id"))
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"no name field"<<std::endl;
			return std::move(error_return(error));
		}

		auto author_id_json = data["id"];

		if (!author_id_json.isInt())
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"name is not string"<<std::endl;
			return std::move(error_return(error));
		}

		result<int> res = pm_lib->change_author(std::unique_ptr<Author>(new Author(author_id_json.asInt(), author_name_json.asString())));

		if (res.m_code == result_code::OK)
		{
			Json::Value data;
			data["id"] = res.m_object;

			return std::move(make_resp(data, ServerStatus::ok()));
		}
		else
		{
			std::cout<<"lib error"<<std::endl;
			return std::move(error_return(ServerStatus::lib_error()));
		}
	};

std::string Server::delete_author(const Json::Value& data)
	{
		if (!data.isMember("id"))
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"no name field"<<std::endl;
			return std::move(error_return(error));
		}

		auto author_id_json = data["id"];

		if (!author_id_json.isInt())
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"name is not string"<<std::endl;
			return std::move(error_return(error));
		}

		result_code res = pm_lib->delete_author(author_id_json.asInt());

        if (res == result_code::OK)
		{
			Json::Value data;
			data["id"] = author_id_json.asInt();

			return std::move(make_resp(data, ServerStatus::ok()));
		}
		else
		{
			std::cout<<"lib error"<<std::endl;
			return std::move(error_return(ServerStatus::lib_error()));
		}
	};

std::string Server::add_book(const Json::Value& data)
	{
		if (!data.isMember("author_id"))
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"no author id field"<<std::endl;
			return std::move(error_return(error));
		}

		auto author_id_json = data["author_id"];

		if (!author_id_json.isInt())
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"author id is not int"<<std::endl;
			return std::move(error_return(error));
		}

		if (!data.isMember("title"))
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"no title field"<<std::endl;
			return std::move(error_return(error));
		}

		auto title_json = data["title"];

		if (!title_json.isString())
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"title is not string"<<std::endl;
			return std::move(error_return(error));
		}

		auto author_res = pm_lib->get_author_by_id(author_id_json.asInt());

        if (author_res.m_code != result_code::OK)
		{
			std::cout<<"lib error get author"<<std::endl;
			return std::move(error_return(ServerStatus::lib_error()));
		}

		auto res = pm_lib->add_book(std::unique_ptr<Book>(new Book(1, title_json.asString(), author_res.m_object)));

		if (res.m_code == result_code::OK)
		{
			Json::Value data;
			data["id"] = res.m_object;

			return std::move(make_resp(data, ServerStatus::ok()));
		}
		else
		{
			std::cout<<static_cast<std::underlying_type<result_code>::type>(res.m_code)<<std::endl;
			std::cout<<"lib error"<<std::endl;
			return std::move(error_return(ServerStatus::lib_error()));
		}
	};

std::string Server::get_book(const Json::Value& data)
	{
		if (!data.isMember("id"))
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"no id field"<<std::endl;
			return std::move(error_return(error));
		}

		auto book_id_json = data["id"];

		if (!book_id_json.isInt())
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"id is not int"<<std::endl;
			return std::move(error_return(error));
		}

		auto res = pm_lib->get_book_by_id(book_id_json.asInt());

        if (res.m_code == result_code::OK)
		{
			Json::Value data;
			data["id"] = res.m_object->get_book_id();
			data["title"] = res.m_object->get_book_title();
			data["author_id"] = res.m_object->get_author_id();

			return std::move(make_resp(data, ServerStatus::ok()));
		}
		else
		{
			std::cout<<"lib error"<<std::endl;
			return std::move(error_return(ServerStatus::lib_error()));
		}
	};

std::string Server::change_book(const Json::Value& data)
	{
		if (!data.isMember("id"))
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"no book id field"<<std::endl;
			return std::move(error_return(error));
		}

		auto book_id_json = data["id"];

		if (!book_id_json.isInt())
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"book id is not int"<<std::endl;
			return std::move(error_return(error));
		}

		if (!data.isMember("author_id"))
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"no author id field"<<std::endl;
			return std::move(error_return(error));
		}

		auto author_id_json = data["author_id"];

		if (!author_id_json.isInt())
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"author id is not int"<<std::endl;
			return std::move(error_return(error));
		}

		if (!data.isMember("title"))
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"no title field"<<std::endl;
			return std::move(error_return(error));
		}

		auto title_json = data["title"];

		if (!title_json.isString())
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"title is not string"<<std::endl;
			return std::move(error_return(error));
		}

		auto author_res = pm_lib->get_author_by_id(author_id_json.asInt());

        if (author_res.m_code != result_code::OK)
		{
			std::cout<<"lib error get author"<<std::endl;
			return std::move(error_return(ServerStatus::lib_error()));
		}

		auto res = pm_lib->change_book(std::unique_ptr<Book>(new Book(book_id_json.asInt(), title_json.asString(), author_res.m_object)));

		if (res.m_code == result_code::OK)
		{
			Json::Value data;
			data["id"] = res.m_object;

			return std::move(make_resp(data, ServerStatus::ok()));
		}
		else
		{
			std::cout<<static_cast<std::underlying_type<result_code>::type>(res.m_code)<<std::endl;
			std::cout<<"lib error"<<std::endl;
			return std::move(error_return(ServerStatus::lib_error()));
		}
	};

std::string Server::delete_book(const Json::Value& data)
	{
		if (!data.isMember("id"))
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"no id field"<<std::endl;
			return std::move(error_return(error));
		}

		auto book_id_json = data["id"];

		if (!book_id_json.isInt())
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"id is not int"<<std::endl;
			return std::move(error_return(error));
		}

		result_code res = pm_lib->delete_book(book_id_json.asInt());

        if (res == result_code::OK)
		{
			Json::Value data;
			data["id"] = book_id_json.asInt();

			return std::move(make_resp(data, ServerStatus::ok()));
		}
		else
		{
			std::cout<<"lib error"<<std::endl;
			return std::move(error_return(ServerStatus::lib_error()));
		}
	};

std::string Server::get_all_author_books(const Json::Value& data)
	{
		if (!data.isMember("id"))
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"no name field"<<std::endl;
			return std::move(error_return(error));
		}

		auto author_id_json = data["id"];

		if (!author_id_json.isInt())
		{
			std::string error = ServerStatus::parser_error();
			std::cout<<"name is not string"<<std::endl;
			return std::move(error_return(error));
		}

		auto res = pm_lib->get_authors_books(author_id_json.asInt());

        if (res.m_code == result_code::OK)
		{
			Json::Value data;
			for (auto book : res.m_object)
			{
				Json::Value book_json;
				book_json["id"] = book->get_book_id();
				book_json["title"] = book->get_book_title();
				book_json["author_id"] = book->get_author_id();
				data["books"].append(book_json);
			}
			return std::move(make_resp(data, ServerStatus::ok()));
		}
		else
		{
			std::cout<<"lib error"<<std::endl;
			return std::move(error_return(ServerStatus::lib_error()));
		}
	};

std::string Server::make_resp(Json::Value& data, const std::string& status)
	{
		Json::Value root;
		root["status"] = status;
		root["data"] = data;

		return std::move(json_to_string(root) + "\v");
	};

std::string Server::error_return(const std::string& error)
	{
		Json::Value root;
		root["status"] = error;

		return std::move(json_to_string(root) + "\v");
	};

std::string Server::proc_msg(const std::string& msg)
    {
        Json::Value root;
        Json::Reader reader;

        if (!reader.parse(msg.c_str(), root))
        {
        	std::string error = ServerStatus::parser_error();
        	std::string error_msg = error_return(error);
        	std::cout<<"parse error"<<std::endl;
        	return std::move(error_msg);
       	}

       	if (!root.isMember("command"))
       	{
       		std::string error = ServerStatus::parser_error();
        	std::string error_msg = error_return(error);
        	std::cout<<"command not found"<<std::endl;
        	return std::move(error_msg);
       	}

       	auto command_json = root["command"];

       	if (!command_json.isString())
       	{
       		std::string error = ServerStatus::parser_error();
        	std::string error_msg = error_return(error);
        	std::cout<<"command is not string"<<std::endl;
        	return std::move(error_msg);
       	}

       	auto command = m_commands.find(command_json.asString());
       	if (command == m_commands.end())
       	{
       		std::string error = ServerStatus::unknown_command();
        	std::string error_msg = error_return(error);
        	std::cout<<"unknown command"<<std::endl;
        	return std::move(error_msg);
       	}

       	if (!root.isMember("data"))
		{
			std::string error = ServerStatus::parser_error();
			std::string error_msg = error_return(error);
			std::cout<<"no data field"<<std::endl;
			return std::move(error_msg);
		}

		auto data = root["data"];

		if (!data.isObject())
		{
			std::string error = ServerStatus::parser_error();
			std::string error_msg = error_return(error);
			std::cout<<"data is not dict"<<std::endl;
			return std::move(error_msg);
		}

       	std::string resp;

       	switch(command->second)
       	{
			case server_command::ADD_AUTHOR:
				resp = add_author(data);
				break;
			case server_command::GET_AUTHOR_BY_ID:
			    resp = get_author(data);
				break;
			case server_command::CHANGE_AUTHOR:
				resp = change_author(data);
				break;
			case server_command::DELETE_AUTHOR_BY_ID:
				resp = delete_author(data);
				break;
			case server_command::ADD_BOOK:
				resp = add_book(data);
				break;
			case server_command::GET_BOOK:
				resp = get_book(data);
				break;
			case server_command::CHANGE_BOOK:
				resp = change_book(data);
				break;
			case server_command::DELETE_BOOK:
				resp = delete_book(data);
				break;
			case server_command::GET_ALL_AUTHOR_BOOKS:
				resp = get_all_author_books(data);
				break;
            default:
                std::string error = ServerStatus::server_error();
                std::cout<<"SERVER ERROR"<<std::endl;
                resp = error_return(error);
       	}

       	return std::move(resp);
    };

void Server::stop()
{
    m_work = false;
}

void Server::init_socket(int port)
{
	/*
	 * TODO Как раз всё то о чём я писал в комментарии в socket.h на 13ой строке, все эти вызовы можно(нужно) заменить одним
	 */
    pm_server_socket->create_socket_fd();
    pm_server_socket->fill_addr(port);
    pm_server_socket->set_in_addr();
    pm_server_socket->bind_socket();
    pm_server_socket->listen_socket();
}

void Server::run()
{
    std::shared_ptr<Socket> client_socket;
    while (m_work)
    {
        /*
         * TODO Вместо проверки флага m_has_connection проще проверять умный указатель client_socket -- если в нём что-то есть
         * то соединение присутствует, если нет, то нет.
         */
        if (!m_has_connection)
        {
            auto accept_res = pm_server_socket->accept_socket();
            if (!accept_res)
            {
                continue;
            }
            client_socket = accept_res.m_object;
            m_has_connection = true;
        }

        auto recv_msg = client_socket->recv_msg();

        if (recv_msg.m_data.length() == 0)
        {
            sleep(1);
        }
        else
        {
            std::string msg;
            for (char& c : recv_msg.m_data)
            {
                std::cout<<c;
                if (c == '\v')
                {
                    if (msg == "pong")
                    {
                        msg = "";
                        continue;
                    }
                    std::string resp = proc_msg(msg);
                    std::cout<<"resp:"<<std::endl<<resp<<std::endl;
                    address addr;
                    if (recv_msg.m_address)
                    {
                        addr = recv_msg.m_address.value();
                    }
                    message send_msg(resp, addr);
                    auto send_res = client_socket->send_msg(send_msg);
                    if (send_res != result_code::OK)
                    {
                        m_has_connection = false;
                    }
                    msg = "";
                    continue;
                }
                msg += c;
            }
        }
        /*
         * TODO В C++11 проще пользоваться утилитами для работы со временем из chrono.
         * Кроме того не стоит много раз вызывать функцию получения текущего времени, предпочтительнее вызвать её один раз и запомнить результат
         */
        std::cout<<"pinger: "<< m_ping_timer<<" time: "<<time(NULL)<<std::endl;
        if (m_has_connection && m_ping_timer < time(NULL))
        {
            m_ping_timer = time(NULL) + 10;
            /*
             * TODO Я так понимаю, что в случае UDP реализация сокета вернёт не совсем тот адрес что нам нужно
             */
            address addr = client_socket->return_address();
            std::string ping("ping\v");
            message send_msg(ping, addr);
            auto send_res = client_socket->send_msg(send_msg);
            if (send_res != result_code::OK)
            {
                /*
                 * TODO Ты не сбрасываешь client_socket после детектирования разрыва соединения, что приводит к неконсистентному состоянию (несоответсвие значения флага m_has_connection и значения client_socket)
                 * и вносит путаницу, кроме того, деструктор сокета может освобождать какие-либо ресурсы
                 */
                m_has_connection = false;
            }
        }
    }
}
