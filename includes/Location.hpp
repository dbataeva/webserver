#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Pars_conf.hpp"

class Location
{
private:
    
public:
    Location                    location();
    std::string                 path;
    std::vector<std::string>    method;
    std::string                 root;
    std::string                 index;
    bool                        autoindex;
    std::string                 cgi;
    bool                        authentication;
    std::string                 redirect;
    int                         max_body;
    int                         count_local;
    int                         no_cgi;
    int                         flag_er;
    int                         flag_er_auto;
	int							flag_root_serv;
    int							flag_redirect;

    Location() : path(std::string()), method(), root(std::string()), index(std::string()), autoindex(), \
    cgi(std::string()), authentication(), redirect(std::string()), max_body(), count_local(0), no_cgi(0), \
    flag_er(0), flag_er_auto(-1), flag_root_serv(0), flag_redirect(0) {};
    Location(const Location &value) 
	{
		*this = value;
	};
    ~Location(){};
    Location&			operator=(const Location& value)
	{
		if (this != &value)
		{
            this->path = value.path;
			this->method = value.method;
			this->root = value.root;
			this->index = value.index;
			this->autoindex = value.autoindex;
            this->cgi = value.cgi;
            this->authentication = value.authentication;
            this->redirect = value.redirect;
            this->max_body = value.max_body;
            this->count_local = value.count_local;
            this->no_cgi = value.no_cgi;
            this->flag_er = value.flag_er;
            this->flag_er_auto = value.flag_er_auto;
			this->flag_root_serv = value.flag_root_serv;
            this->flag_redirect = value.flag_redirect;
		}
		return (*this);
	}
    std::string         pars_path(std::string str_local);
    std::vector<int>    pars_method(std::string str_local);
    std::string	        pars_root(std::string str_local);
    std::string	        pars_index(std::string str_local);
    bool	            pars_autoindex(std::string str_local);
    std::string	        pars_authentication(std::string str_local);
    int	                pars_max_body(std::string str_local);
    bool	            pars_redirect(std::string str_local);
    std::string	        pars_cgi(std::string str_local);

};

#endif