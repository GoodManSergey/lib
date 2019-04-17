#pragma once
#include<string>

class ServerStatus
{
	public:
	static std::string parser_error()
	{
		return std::move("PARSER_ERROR");
	}

	static std::string unknown_command()
	{
		return std::move("UNKNOWN_COMMAND");
	}

	static std::string ok()
	{
		return std::move("OK");
	}

	static std::string lib_error()
	{
		return std::move("LIB_ERROR");
	}

	static std::string server_error()
	{
		return std::move("SERVER_ERROR");
	}
};
