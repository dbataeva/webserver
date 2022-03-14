#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <string.h>
#include <map>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <algorithm>
#include <cstdlib>

#include "parser_env.hpp"
#include "Pars_conf.hpp"
#include "Autoindex.hpp"
#include "error.hpp"

std::map<std::string, std::string> save_types();
std::map<std::string, std::string> save_responses();

class response {

	private:
		char		*str_to_send;
		size_t		len_to_send;
		char		**envp;
		std::string error_page;
		std::string header;
		std::string body;
		std::string method;
		Server		serv;
		static long long id;
		static std::map<std::string, std::string> mime_types;
		static std::map<std::string, std::string> responses_status_codes;

		response();
		response(const response &to_copy);
		const response &operator=(const response &to_assignation);

		std::string	define_a_value(std::string to_find);
		void		do_get(std::string &location, int client);
		std::string get_uri(char **envp);
		void		form_response();
		void		do_cgi(std::string &uri, std::string extension, int client);
		std::vector<std::string> get_path(const char *path, char separator, int i, int j);
		std::vector<std::string> get_locations(std::string &path, char separator);
		std::string get_program(std::vector<std::string> path, std::string extension);
		std::string	form_error_page(std::string code);
		void		autoindex_on(char **envp);
		void		parse_autoindex();
		void		make_html();
		std::string	define_location(std::string big_path, int client);
		std::string	do_autoindex(std::string &location, std::string &root, int client);
		bool		check_methods(std::vector<std::string> &allowed_methods);
		void		form_error(std::string code);
		std::string	get_extension(std::string path);
		std::string	not_found(std::string &location, bool &autoindex);
		std::string make_redirect(std::string &redirect);
		char 		*find_range(void);
		int 		from_str_to_char(int len);
		int			define_range(char *range, unsigned int *start, unsigned int *finish);
		void		write_to_buffer(std::string uri, int client);
		std::string	parse_cgi(char *b, int client);
		std::string client_redirect_response_with_document(std::string &cgi_header, int location, std::string &buf, int header_end);
		std::string client_redirect_response(std::string &cgi_header, int location);
		std::string document_response(std::string &cgi_header, int type, std::string &buf, int header_end);
		void		make_response(int client);
		std::string local_redirect_response(std::string &cgi_header, int location, int client);
		void		change_envp(std::string &str);
		void		form_header(std::string content_type, std::string uri, int client);
		void 		define_server(std::string host, Server **serv, int count_serv);
		void		do_delete(std::string &location);
		std::string	do_post(int client);
		std::string parse_multipart(std::string file_name, int client);
		std::string parse_binary(std::string full_path, int client, std::string type);
		void		do_partial_content(char *range, std::string &location);

	public:
		response(std::string &request, Server &this_serv, int client);
		~response();

		char	*get_char();
		size_t	get_len();
};

#endif