#pragma once
#include<string>

class Author
{
    public:
    Author(int id, const std::string& name);
    int get_author_id() const;
    const std::string& get_name() const;
    void set_author_id(int id);
    void set_name(const std::string& name);
    
    private:
    std::string m_name;
    int m_author_id;

};
