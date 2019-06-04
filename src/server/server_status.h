#pragma once
#include<string>

class ServerStatus
{
	public:
	static std::string parser_error()
	{
		return std::move(std::string("PARSER_ERROR"));
	}

	static std::string unknown_command()
	{
		return std::move(std::string("UNKNOWN_COMMAND"));
	}

	static std::string ok()
	{
		return std::move(std::string("OK"));
	}

	static std::string lib_error()
	{
		return std::move(std::string("LIB_ERROR"));
	}

	static std::string server_error()
	{
		return std::move(std::string("SERVER_ERROR"));
	}
};
