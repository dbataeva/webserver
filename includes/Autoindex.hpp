#ifndef AUTIONDEX_HPP
# define AUTIONDEX_HPP

# include <iostream>
# include <vector>
# include <dirent.h>
# include <ctime>
# include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "parser_env.hpp"

class Autoindex
{
private:
    std::string                 body;
    std::vector<std::string>    content_dir;
    std::vector<std::string>    data;
    Autoindex();
    void    create_body(std::string &path);
public:
    Autoindex(std::string &root, std::string &path);
    Autoindex(Autoindex const &value);
    Autoindex &operator=(Autoindex const &value);
    ~Autoindex();
    std::string getTime();
    std::string getData(std::string &path);
    std::string const &getBody();
};

#endif