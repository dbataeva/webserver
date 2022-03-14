#ifndef ERROR_HPP
# define ERROR_HPP

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
#include <string>


class Error
{
private:
    std::string                 body;
	std::string					number_error;
    std::string					error_page;
    Error();
	
    void    create_body(const std::string number_error);
public:
    Error(Error const &value);
	Error(std::string _number_error, std::string _error_page);
    Error &operator= (Error const &value);
    ~Error();
    std::string const &getBody();
	
};

#endif