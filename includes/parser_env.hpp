#ifndef PARSER_ENV_HPP
#define PARSER_ENV_HPP

#include "vector"
#include "map"
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

struct parser_env
{
    std::string content_length;
    std::string content_type;
    std::string path_info;
    std::string path_translated;
    std::string ip;
    std::string host;
    std::string user;
    std::string ident;
    std::string data;
    std::string metod;
    std::string recv_uri;
    std::string script_name;
    std::string server_name;
    std::string port;
    std::string protocol;
    std::string body;
    std::string cookie;
};

char**  add_env(std::string s1);
void    get_body(std::string s1, std::string *body);
void    get_body_chunked(std::string s1, std::string *body);


#endif