#ifndef PARS_CONF_HPP
#define PARS_CONF_HPP

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <netdb.h>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <typeinfo>
#include <cstdlib>

#include "Location.hpp"

class Server
{
private:

public:
	Server									serv();
	std::string								root_serv;
	std::vector<int>						fd_serv;
	int										serv_default;
    std::vector<std::string>				str_conf;
	std::string								ip;
	std::vector<int>						port;
	std::string								server_name;
	std::string								error_page;
	std::vector<Location*>					locations;
	std::string                 			redirect_serv;
	int										count_serv;
	int										count_local;
	int										flag_error;
	int										flag;
	int										flag_local;
	int                         			path_matches;
	int										flag_redirect_serv;
	int										flag_local_full;

    Server() : root_serv(), serv_default(0), str_conf(), ip(std::string()), port(), server_name(std::string()), error_page(std::string()), \
	redirect_serv(std::string()), count_serv(0), count_local(0), flag_error(0), flag(0), flag_local(0), path_matches(0), flag_redirect_serv(0), flag_local_full(0)  {};
	Server&			operator=(const Server& value)
	{
		if (this != &value)
		{
			this->root_serv = value.root_serv;
			this->fd_serv = value.fd_serv;
			this->serv_default = value.serv_default;
			this->str_conf = value.str_conf;
			this->ip = value.ip;
			this->port = value.port;
			this->server_name = value.server_name;
			this->error_page = value.error_page;
			this->locations = value.locations;
			this->redirect_serv = value.redirect_serv;
			this->count_serv = value.count_serv;
			this->count_local = value.count_local;
			this->flag_error = value.flag_error;
			this->flag = value.flag;
			this->flag_local = value.flag_local;
			this->path_matches = value.path_matches;
			this->flag_redirect_serv = value.flag_redirect_serv;
			this->flag_local_full = value.flag_local_full;
		}
		return (*this);
	}
	Server(const Server &value) 
	{
		*this = value;
	};

    ~Server() {};
    int							pars_listen(std::string str_in_conf, Server *serv);
    int							pars_error_page(std::string str_in_conf, Server *serv);
    int							pars_server_name(std::string str_in_conf, Server *serv);
    int							ft_count_serv(std::string argv, Server *serv);
	std::vector<std::string>	conf_in_vector(std::string argv);
    Server						**parser_conf(std::string argv, Server *serv);
    
};


#endif