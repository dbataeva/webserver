#include "../includes/response.hpp"

std::map<std::string, std::string> response::mime_types = save_types();
std::map<std::string, std::string> response::responses_status_codes = save_responses();
long long response::id = 0;

std::vector<std::string> response::get_path(const char *path, char sep, int i, int j) {
	std::vector<std::string> loc;
	while (path[i]) {
		if (path[i] == sep)
			j++;
		i++;
	}
	if (j == 0) {
		loc.push_back(path);
		return (loc);
	}
	j += 2;
	int begin = 0;
	std::string p(path);
	int len;
	unsigned long separator = p.find(sep, begin);
	while (separator != p.npos) {
		len = separator - begin;
		loc.push_back(p.substr(begin, len));
		begin = begin + len + 1;
		separator = p.find(sep, begin);
		i++;
	}
	len = separator - begin;
	loc.push_back(p.substr(begin, len));
	return (loc);
}

std::vector<std::string> response::get_locations(std::string &path, char sep) {
	std::vector<std::string> loc;
	int begin = 0;
	int len;
	int i = 0;
	unsigned long separator = path.find(sep, begin);
	while ((separator != path.npos)) {
		len = separator - begin;
		if (len == 0) {
			separator = path.find(sep, begin + 1);
			continue ;
		}
		loc.push_back(path.substr(begin, len));
		i++;
		begin = begin + len;
		separator = path.find(sep, begin);
	}
	len = separator - begin;
	if (i == 0)
		loc. push_back(path);
	else
		loc.push_back(path.substr(begin, len));
	return (loc);
}

int response::from_str_to_char(int len) {
	this->len_to_send = len;
	this->str_to_send = new char [this->len_to_send];
	int j = 0;
	while (this->header[j]) {
		this->str_to_send[j] = this->header[j];
		j++;
	}
	return (j);
}

std::string	response::form_error_page(std::string code) {
 	std::map<std::string, std::string>::iterator description = responses_status_codes.find(code);
	this->header = "HTTP/1.1 ";
	this->header += code;
	this->header += " ";
	this->header += description->second;
	this->header += "\nContent-Type: text/html\nContent-Length: ";

	code += " ";
	code += description->second;
	Error er(code, this->serv.error_page);
	std::string body = er.getBody();
	std::ostringstream convert;
	convert << body.length();
	this->header += convert.str();
	this->header += "\nConnection: close\n\n";
	this->header += body;
	this->from_str_to_char(header.length());
	return (".");
}

void	response::form_error(std::string code) {
 	std::map<std::string, std::string>::iterator description = responses_status_codes.find(code);
	this->header = "HTTP/1.1 ";
	this->header += code;
	this->header += " ";
	this->header += description->second;
	this->header += "\nContent-Length: 0\nConnection: close\n\n";
	int len = this->header.length() + 1;
	int j = this->from_str_to_char(len);
	this->str_to_send[j] = '\0';
	return ;
}

std::string response::get_program(std::vector<std::string> path, std::string extension) {
	struct stat	buf[4096];
	std::string program;
	for (unsigned long i = 0; i < path.size(); i++) {
		program = path[i];
		program += "/";
		program += extension;
		if (!stat(program.c_str(), buf))
			break ;
	}
	return (program);
}

std::string response::client_redirect_response_with_document(std::string &cgi_header, int location, std::string &buf, int header_end) {
	unsigned long status = cgi_header.find("Status: ", 0);
	if (status == cgi_header.npos) {
		this->form_error_page("418");
		return (".");
	}

	unsigned long type = cgi_header.find("Content-Type: ", 0);
	if (type == cgi_header.npos) {
		this->form_error_page("418");
		return (".");
	}
	int status_end = cgi_header.find("\n", status);
	std::string s = cgi_header.substr(status + 8, status_end - status + 7);
	this->header += s;
	int type_end = cgi_header.find("\n", type);
	std::string t = cgi_header.substr(type, type_end - type + 1);
	this->header += t;
	int location_end = cgi_header.find("\n", location);
	std::string l = cgi_header.substr(location, location_end - location + 1);
	this->header += l;
	this->header += "\nConnection: close\n\n";
	return (buf.substr(header_end + 2, buf.length() - header_end + 2));
}

std::string response::client_redirect_response(std::string &cgi_header, int location) {
	this->header += "302 Found\n";
	unsigned long location_end = cgi_header.find("\n", location);
	if (location_end == cgi_header.npos)
		location_end = cgi_header.find("\0", location);
	std::string l = cgi_header.substr(location, location_end - location);
	this->header += l;
	this->header += "\nConnection: close\n\n";
	return (".");
}

std::string response::document_response(std::string &cgi_header, int type, std::string &buf, int header_end) {
	unsigned long status = cgi_header.find("Status: ");
	if (status != cgi_header.npos) {
		int status_end = cgi_header.find("\n", status);
		std::string s = cgi_header.substr(status + 8, status_end - status + 7);
		this->header += s;
	}
	else
		this->header += "201 Created\n";
	unsigned long type_end = cgi_header.find("\n", type);
	if (type_end == cgi_header.npos) {
		type_end = cgi_header.length();
	}
	std::string t = cgi_header.substr(type, type_end - type + 1);
	this->header += t;
	unsigned long session_and_cookie = cgi_header.find("Set-cookie: ", 0);
	if (session_and_cookie != cgi_header.npos)
		this->header += "Content-Length: ";
	else
		this->header += "\nContent-Length: ";
	std::string body = buf.substr(header_end + 2, buf.length() - header_end + 2);
	std::ostringstream convert;
	convert << body.length();
	this->header += convert.str();
	this->header += "\nConnection: close";
	if (session_and_cookie != cgi_header.npos) {
		this->header += "\n";
		unsigned long session_and_cookie_end = cgi_header.find("\n", session_and_cookie);
		this->header += cgi_header.substr(session_and_cookie, session_and_cookie_end - session_and_cookie);
	}
	this->header += "\n\n";
	return (body);
}

void	response::change_envp(std::string &str) {
	for (int i = 0; this->envp[i]; i++) {
		std::string s(this->envp[i]);
		size_t find_value = s.find(str, 0);
		if (find_value != s.npos) {
			free (this->envp[i]);
			int len = str.length() + 1;
			this->envp[i] = new char [len];
			memset(this->envp[i], '\0', len);
			for (int j = 0; j < len; j++) {
				this->envp[i][j] = str[j];
			}
			return ;
		}
	}
	return ;
}

std::string response::local_redirect_response(std::string &cgi_header, int location, int client) {
	unsigned long location_end = cgi_header.find("\n", location);
	if (location_end == cgi_header.npos)
		location_end = cgi_header.find("\0", location);
	std::string l = cgi_header.substr(location, location_end - location);
	unsigned long end_path_info = l.find("?", 0);
	if (end_path_info == l.npos)
		end_path_info = l.length();
	std::string query_string("QUERY_STRING=");
	std::string path_info("PATH_INFO=");
	path_info += l.substr(0, end_path_info);
	if (end_path_info != l.length()) {
		if (++end_path_info != l.length()) {
			query_string += l.substr(end_path_info, l.length() - end_path_info);
		}
	}
	this->change_envp(path_info);
	this->change_envp(query_string);
	this->make_response(client);
	return (".");
}

std::string	response::parse_cgi(char *b, int client) {
	if (!b) {
		this->form_error_page("418");
		return (".");
	}
	std::string buf(b);
	std::string cgi_header;
	unsigned long header_end = buf.find("\n\n");
	if (header_end != buf.npos) {
		cgi_header = buf.substr(0, header_end);
	}
	else {
		header_end = buf.length();
		cgi_header = buf;
	}
	unsigned long location = cgi_header.find("Location: ", 0);
	if (location != cgi_header.npos) {
		if ((cgi_header.find("://", location) != cgi_header.npos)) {
			if (buf.length() > header_end + 2)
				return (this->client_redirect_response_with_document(cgi_header, location, buf, header_end));
			else // no body
				return (this->client_redirect_response(cgi_header, location));
		}
		else //обработка запроса так, будто в запросе были такие как в location path & query string
			return (this->local_redirect_response(cgi_header, location, client));
	}
	unsigned long type = cgi_header.find("Content-Type: ", 0);
	if (type == cgi_header.npos) {
		this->form_error_page("418");
		return (".");
	}
	return (this->document_response(cgi_header, type, buf, header_end));
}

void	response::write_to_buffer(std::string uri, int client) {
	std::ifstream fs;
	fs.open(uri.c_str(), std::ios::binary);
	if (!fs.is_open()) {
		form_error_page("404");
		return ;
	}
	fs.seekg(0, fs.end);
	int length = fs.tellg();
	fs.seekg(0, fs.beg);

	if (uri == "utils/.cgi") {
		char *b = new char [length + 1];
		fs.read(b, length);
		b[length] = '\0';
		std::string cgi = this->parse_cgi(b, client);
		delete [] b;
		if (cgi == ".")
			return ;
		
		this->len_to_send = this->header.length() + cgi.length();
		this->str_to_send = new char [this->len_to_send];
		unsigned long i;
		for (i = 0; i < header.length(); i++) {
			this->str_to_send[i] = this->header[i];
		}
		for (unsigned long j = 0; j < cgi.length(); j++) {
			this->str_to_send[i] = cgi[j];
			i++;
		}
	}
	else {
		this->len_to_send = this->header.length() + length;
		this->str_to_send = new char [this->len_to_send];
		unsigned long i;
		for (i = 0; i < header.length(); i++) {
			this->str_to_send[i] = this->header[i];
		}
		fs.read(&this->str_to_send[i], length);
	}
	fs.close();
	return ;
}
		
void	response::do_cgi(std::string &uri, std::string extension, int client) {
	char *argv[3];
	std::string program;
	if ((extension != "0") && (extension != "cgi")) {
		std::vector<std::string> path = this->get_path(getenv("PATH"), ':', 0, 0);
		program = get_program(path, extension);
		argv[0] = (char *)(program.c_str());
		argv[1] = (char *)(uri.c_str());
	}
	else {
		argv[0] = (char *)(uri.c_str());
		argv[1] = 0;
	}
	argv[2] = 0;	
	int pipe_to_child[2];
	int pipe_from_child[2];
	pipe(pipe_to_child);
	pipe(pipe_from_child);
	pid_t pid = fork();

	if (pid == 0) {
		// std::cout << "in child" << std::endl;
		close (pipe_to_child[1]);
		close (pipe_from_child[0]);
		dup2(pipe_to_child[0], 0);
		dup2(pipe_from_child[1], 1);
		close(pipe_to_child[0]);
		close(pipe_from_child[0]);
		if (execve(argv[0], argv, this->envp) == -1) {
			std::cout << "execve error" << std::endl;
			exit (1);
		}
	}
	else if (pid < 0)
		std::cout << "2server error" << std::endl;
	else {
		sleep(2);
		// std::cout << "in parent" << std::endl;
		write(pipe_to_child[1], this->body.c_str(), this->body.length());
		close (pipe_to_child[1]);
		char buf[1024];
		int fd_out = open("utils/.cgi", O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU);
		int r;
		while ((r = read(pipe_from_child[0], buf, 1023)) > 0) {
			buf[r] = '\0';
			write(fd_out, buf, r);
			if (r < 1023)
				break ;
		}
		int status = 0;
		wait(&status);
		close (fd_out);
	}
	close (pipe_from_child[0]);
	close (pipe_to_child[0]);
	close (pipe_from_child[1]);
	this->write_to_buffer("utils/.cgi", client);
	return ;
}

std::string response::get_uri(char **envp) {
	int i = 0;
	while (envp[i])
	{
		std::string str(envp[i]);
		size_t ravno = str.find("PATH_INFO=", 0);
		if (ravno != str.npos) {
			ravno = str.find("=", 0);
			return (str.substr(++ravno, str.length()));
		}
		i++;
	}
	return ("no uri");
}

void	response::form_header(std::string content_type, std::string uri, int client) {
	std::ifstream fs;
	this->header += "200 OK\nContent-Type: ";
	this->header += content_type;
	this->header += "\nContent-Length: ";
	fs.open(uri.c_str(), std::ios::binary);
	if (!fs.is_open()) {
		form_error("404");
		return ;
	}
	fs.seekg(0, fs.end);
	int length = fs.tellg();
	fs.seekg(0, fs.beg);
	fs.close();
	std::ostringstream convert;
	convert << length;
	this->header += convert.str();
	this->header += "\nConnection: close\n\n";
	this->write_to_buffer(uri, client);
	return ;
}

void	response::do_get(std::string &location, int client) {
	std::string extension = this->get_extension(location); // выделяем расширение файла
	if (extension == ".")
		this->do_cgi(location, "0", client);
	else {
		std::map<std::string, std::string>::iterator type = mime_types.find(extension); // ищем в mime_types тип
		if (type != mime_types.end()) { // нашелся тип
			this->form_header(type->second, location, client);
		}
		else {
			if (extension == "py")
				extension = "python3.7";
			else if (extension == "cgi")
				this->do_cgi(location, "0", client);
			this->do_cgi(location, extension, client);
		}
	}
	return ;
}

std::string response::define_a_value(std::string to_find) {
	int i = 0;

	while (this->envp[i])
	{
		std::string str(this->envp[i]);
		size_t find_value = str.find(to_find, 0);
		if (find_value != str.npos) {
			find_value = str.find("=", 0);
			return (str.substr(++find_value, str.length()));
		}
		i++;
	}
	return ("undefind method");
}

response::response () : str_to_send(0), len_to_send(0) {}

// std::string	response::do_autoindex(std::string &location, std::string &for_autoindex, int client) {
// 	Autoindex a(location, for_autoindex);
// 	int fd_out = open("utils/.autoindex", O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU);
// 	close (fd_out);
// 	std::string b = a.getBody();
// 	std::ofstream fs;
// 	fs.open("utils/.autoindex", std::ofstream::out | std::ofstream::binary);
// 	fs.write(b.c_str(), b.length());
// 	fs.close();
// 	this->form_header("text/html", "utils/.autoindex", client);
// 	return (".");
// }

bool	response::check_methods(std::vector<std::string> &allowed_methods) {
	for (std::vector<std::string>::iterator it = allowed_methods.begin(); it != allowed_methods.end(); it++) {
		if (this->method == *it)
			return (true);
	}
	return (false);
}

std::string response::get_extension(std::string path) {
	unsigned long slesh = path.rfind("/");
	size_t dot;
	if (slesh != path.npos) {
		dot = path.find('.', slesh);
	}
	else
		dot = path.rfind('.');
	if (dot != path.npos)
		return (path.substr(++dot, path.length()));
	return (".");
}

std::string response::not_found(std::string &location, bool &autoindex) {
	std::string extension = this->get_extension(location);
	if ((extension == "html") || (this->method == "DELETE"))
		this->form_error_page("404");
	else if (extension == ".") //если нет расширения
		this->form_error_page("404");
	// else if (autoindex)
	// 	this->form_error_page("404");
	else
		this->form_error("404");
	return (".");
}

std::string response::make_redirect(std::string &redirect) {
	this->header += "301 Moved Permanently\nContent-Type: text/html\nContent-Length: 0\nLocation: ";
	this->header += redirect;
	this->header += "\nConnection: close\n\n";
	int len = this->header.length() + 1;
	int j = this->from_str_to_char(len);
	this->str_to_send[j] = '\0';
	return (".");
}

std::string response::parse_multipart(std::string pwd, int client) {
	unsigned long subheader_end = this->body.find("\r\n\r\n");
	unsigned long filename = this->body.find("filename=\"");
	filename += 10;
	unsigned long filename_end = this->body.find("\"", filename);
	std::string name = this->body.substr(filename, filename_end - filename);
	unsigned long content_type = this->body.find("Content-Type: ");
	content_type += 14;
	std::string type;
	for (; this->body[content_type] != '\r'; content_type++) {
		if (this->body[content_type] == ' ')
			break ;
		type.push_back(this->body[content_type]);
	}
	name = "/cgi-bin/file/downloads/" + name;
	this->header += name;
	name = pwd + name;
	int fd = open(name.c_str(), O_CREAT | O_WRONLY, S_IRWXU);
	if (fd < 0) {
		this->form_error_page("500");
		return (".");
	}
	close(fd);
	subheader_end += 4;
	std::string subbody = this->body.substr(subheader_end, this->body.length() - subheader_end);
	std::ofstream fs;
	fs.open(name.c_str(), std::ofstream::out | std::ofstream::binary);
	fs.write(subbody.c_str(), subbody.length());
	fs.close();
	this->header += "\nConnection: close\n\n";
	this->write_to_buffer(name, client);
	return (".");
}

std::string response::parse_binary(std::string full_path, int client, std::string type) { 
	type.erase(--type.end());
	std::map<std::string, std::string>::iterator it;
	for (it = mime_types.begin(); it != mime_types.end(); it++) {
		if (it->second == type)
			break ;
	}
	std::ostringstream convert;
	convert << id;
	std::string file_name = "/cgi-bin/file/downloads/file" + convert.str();
	if (it != mime_types.end())
		file_name = file_name + "." + it->first;
	this->header += file_name;
	file_name = full_path + file_name;
	id++;
	int fd = open(file_name.c_str(), O_CREAT | O_WRONLY, S_IRWXU);
	if (fd < 0) {
		this->form_error_page("500");
		return (".");
	}
	close(fd);
	std::ofstream fs;
	fs.open(file_name.c_str(), std::ofstream::out | std::ofstream::binary);
	fs.write(this->body.c_str(), this->body.length());
	fs.close();
	this->header += "\n\n";
	// this->from_str_to_char(this->header.length());
	this->write_to_buffer(file_name, client);
	return (".");
}

std::string	response::do_post(int client) {
	if (!this->body.length()) {
		this->form_error_page("400");
		return (".");
	}
	this->header += "201 ";
	this->header += this->responses_status_codes.find("201")->second;
	this->header += "\nContent-Lenght: 0\nLocation: ";
	std::string full_path = getenv("PWD");
	std::string chunked = this->define_a_value("HTTP_TRANSFER-ENCODING");
	if (chunked == "undefind method") {
		chunked = this->define_a_value("HTTP_CONTENT-TYPE");
		if (chunked.find("multipart/form-data") != chunked.npos)
			return (this->parse_multipart(full_path, client));
		return(this->parse_binary(full_path, client, chunked));
	}
	std::ostringstream convert;
	convert << id;
	std::string file_name = "/cgi-bin/file/downloads/file" + convert.str();
	this->header += file_name;
	file_name = full_path + file_name;
	id++;
	int fd = open(file_name.c_str(), O_CREAT | O_WRONLY, S_IRWXU);
	if (fd < 0) {
		this->form_error_page("500");
		return (".");
	}
	close(fd);
	std::ofstream fs;
	fs.open(file_name.c_str(), std::ofstream::out | std::ofstream::binary);
	fs.write(this->body.c_str(), this->body.length());
	fs.close();

	this->header += "\nConnection: close\n\n";
	// this->from_str_to_char(this->header.length());
	this->write_to_buffer(file_name, client);
	return (".");
}

std::string response::define_location(std::string big_path, int client) {
	std::string full_path = getenv("PWD");
	this->error_page = full_path + this->serv.error_page;
	if (big_path.length() > 2000)
		return (this->form_error_page("441"));
	std::vector<std::string> path = get_locations(big_path, '/');
	std::string location;
	if (path[0] == "")
		path[0] = "/";
	std::vector<Location *>::iterator it;
	for (it = this->serv.locations.begin(); it != this->serv.locations.end() - 1; it++) {
		if (path[0] + "/" == (*it)->path)
			break ;
	}
	if (it == this->serv.locations.end() - 1) {
		for (it = this->serv.locations.begin(); it != this->serv.locations.end() - 1; it++) {
			if ((*it)->path == "/")
				break ;
		}
	}
	if (big_path.find("/error_page/", 0) != big_path.npos)
		location = big_path.substr(big_path.find("/error_page/", 0));
	else if ((*it)->flag_redirect)
		return (this->make_redirect((*it)->redirect));
	else if (((*it)->max_body > 0) && (this->body.length() > (unsigned int)(*it)->max_body))
		return (this->form_error_page("413"));
	else if ((*it)->flag_root_serv) {
		if ((*it)->root != "/")
			location = (*it)->root;
		for (unsigned long i = 1; i < path.size(); i++) {
			if (path[i] != "/")
				location += path[i];
		}
	}
	else {
		if ((*it)->root != "/")
			location = (*it)->root;
		location += big_path;
	}
	location = full_path + location;
	struct stat	buf[4096];
	if (!this->check_methods((*it)->method))
		return (this->form_error_page("405"));
	if (stat(location.c_str(), buf))
		return (this->not_found(location, (*it)->autoindex));
	if (!S_ISDIR(buf->st_mode))
		return (location);
	else if (this->method == "DELETE")
		return (this->form_error_page("403"));
	else if (this->method == "POST")
		return (this->do_post(client));
	// else if ((*it)->autoindex) {
	// 	if (path[path.size() - 1] == "/")
	// 		path[path.size() - 1] = "";
	// 	return (this->do_autoindex(location, path[path.size() - 1], client));
	// }
	else {
		if (path[0] == "/")
			location += (*it)->index;
		else
			location = location + "/" + (*it)->index;
		if (stat(location.c_str(), buf)) {
			return (this->not_found(location, (*it)->autoindex));
		}
	}
	return (location);
}

char * response::find_range(void) {
	int i = 0;

	while (this->envp[i]) {
		std::string str(this->envp[i]);
		size_t find_range = str.find("HTTP_RANGE=", 0);
		if (find_range != str.npos)
			return (this->envp[i]);
		i++;
	}
	return (0);
}

int	response::define_range(char *range, unsigned int *start, unsigned int *finish) {
	std::string r(range);
	int start_num = r.rfind('=') + 1;
	unsigned long end_of_first_num = r.find('-', 0);
	if (end_of_first_num == r.length())
		*finish = 0;
	else
		*finish = atoi(&r[end_of_first_num]);
	std::string first_num = r.substr(start_num, end_of_first_num - start_num);
	*start = atoi(first_num.c_str());
	return (start_num);
}

void	response::do_partial_content(char *range, std::string &location) {
	std::ifstream fs;
	std::string type;
	unsigned int start;
	unsigned int finish;
	std::string r(range);
	int start_num = this->define_range(range, &start, &finish);
	std::string extension = this->get_extension(location);
	std::map<std::string, std::string>::iterator it = mime_types.find(extension);
	if (it != mime_types.end())
		type = it->second;
	this->header += "206 Partial Content\nContent-Type: ";
	this->header += type;
	this->header += "\nContent-Range: bytes ";
	this->header += r.substr(start_num, r.length() - 1 - start_num);
	fs.open(location.c_str(), std::ios::binary);
	if (!fs.is_open()) {
		form_error("404");
		return ;
	}
	fs.seekg(0, fs.end);
	int length = fs.tellg();
	fs.seekg(0, fs.beg);
	std::ostringstream convert;
	convert << length - 1;
	this->header += convert.str();
	this->header += "/";
	std::ostringstream conver;
	conver << length;
	this->header += conver.str();
	this->header += "\nContent-Length: ";
	unsigned int len;
	if (finish == 0)
		len =  length - start;
	else
		len = finish - start + 1;
	std::ostringstream conv;
	conv << len;
	this->header += conv.str();
	this->header += "\nConnecton: close\n\n";
	int l = this->header.length() + len;
	int i = this->from_str_to_char(l);
	int skip = start - 0;
	char *s = new char [skip];
	fs.read(s, skip);
	delete [] s;
	fs.read(&this->str_to_send[i], len);
	fs.close();
	return ;
}

void	response::do_delete(std::string &location)
{
	remove(location.c_str());
	this->header += "200 ";
	this->header += responses_status_codes.find("200")->second;
	this->header += "\n";
	this->header += "Content-Length: 0\nConnection: close\n\n";
	from_str_to_char(this->header.length());
}

void	response::make_response(int client) {
	char *range = this->find_range();
	std::string location = this->define_location(this->define_a_value("PATH_INFO="), client);
	if (location == ".")
		return ;
	if (range) {
		this->do_partial_content(range, location);
		return ;
	}
	if ((this->method == "GET") || (this->method == "POST"))
		do_get(location, client);
	else if (this->method == "DELETE")
		do_delete(location);
	else
		form_error_page("405");
	return ;
}

response::response(std::string &request, Server &this_serv, int client) {
	this->header = "HTTP/1.1 ";
	this->envp = add_env(request);
	this->serv = this_serv;
	this->method = this->define_a_value("REQUEST_METHOD=");
	if (this->method == "POST")
		get_body(request, &this->body);
	this->make_response(client);
	std::cout << this->header << std::endl;
	return ;
}

response::response(const response &to_copy) {
	this->header = to_copy.header;
	this->len_to_send = to_copy.len_to_send;
	this->method = to_copy.method;
	this->serv = to_copy.serv;
	this->body = to_copy.body;
	this->error_page = to_copy.error_page;
	this->str_to_send = new char [this->len_to_send];
	size_t i;
	for (i = 0; i < this->len_to_send; i++) {
		this->str_to_send[i] = to_copy.str_to_send[i];
	}
	for (i = 0; to_copy.envp[i]; i++);
	i++;
	this->envp = new char * [i];
	for (i = 0; to_copy.envp[i]; i++) {
		int j = strlen(to_copy.envp[i]) + 1;
		this->envp[i] = new char [j];
		for (j = 0; to_copy.envp[i][j]; j++) {
			this->envp[i][j] = to_copy.envp[i][j];
		}
		this->envp[i][j] = 0;
	}
	this->envp[i] = 0;
	return ;
}

const response & response::operator=(const response &to_assignation) {
	if (this != &to_assignation) {
		this->len_to_send = to_assignation.len_to_send;
		if (this->str_to_send)
			delete [] str_to_send;
		this->str_to_send = new char [this->len_to_send];
		memcpy(this->str_to_send, to_assignation.str_to_send, this->len_to_send);
	}
	return (*this);
}

response::~response() {
	for (int i = 0; this->envp[i]; i++) {
		delete [] this->envp[i];
	}
	delete [] this->envp;
	delete [] this->str_to_send;
	this->len_to_send = 0;
}

char * response::get_char() {
	return (this->str_to_send);
}


size_t response::get_len() {
	return (this->len_to_send);
}
