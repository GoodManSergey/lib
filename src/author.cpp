#include"author.h"

Author::Author(int id, const std::string& name): m_author_id(id), m_name(name){};

int Author::get_author_id() const {return m_author_id;};

const std::string& Author::get_name() const {return m_name;};

void Author::set_author_id(int id)
{
	m_author_id = id;
};

void Author::set_name(const std::string& name)
{
	m_name = name;
};
