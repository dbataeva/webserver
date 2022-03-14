#ifndef MAIN_MACOS_HPP
#define MAIN_MACOS_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <string>
#include <fstream>
#include <fcntl.h>
#include <sstream>
#include <map>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <list>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "parser_env.hpp"
#include "response.hpp"
#include "Pars_conf.hpp"

char *make_response(std::string first_line, int fd);
std::map<std::string, std::string> save_types();
std::map<std::string, std::string> save_responses();
int		ft_count_serv_and_local(std::string argv, Server *serv);
Server	**parser_conf(std::string argv, Server *serv);
void	get_body(std::string s1, std::string *body);

#endif