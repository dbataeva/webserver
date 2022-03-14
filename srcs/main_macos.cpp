#include "../includes/main_macos.hpp"

char *make_response(std::string first_line, int fd);
std::map<std::string, std::string> save_types();
std::map<std::string, std::string> save_responses();
int		ft_count_serv_and_local(std::string argv, Server *serv);
Server	**parser_conf(std::string argv, Server *serv);
void	get_body(std::string s1, std::string *body);

int close_sockets(std::map<int, Server> &socket_server) {
	for (std::map<int, Server>::iterator it = socket_server.begin(); it != socket_server.end(); it++) {
        close (it->first);
    }
	return (1);
}

bool check_if_multipart(std::string &request) {
	unsigned long boundary;
	unsigned long end_boundary;
	unsigned long last_boundary;
	std::string b;

	if (request.find("POST") != request.npos) {
		boundary = request.find("Content-Type: multipart/form-data; boundary=", 0);
		if (boundary == request.npos)
			return (true); // это не multipart запрос
		boundary  += 44;
		end_boundary = request.find("\r\n", boundary);
		b = request.substr(boundary, end_boundary - boundary);
		b += "--\r\n";
		last_boundary = request.rfind(b);
		if (last_boundary == request.npos) {
			return (false); // завершающего boundary в запросе нет - нет тела
		}
		return (true); // завершающий boundary в запросе - тело пришло все
	}
	return (true); // это не multipart запрос
}

bool check_if_chunked(std::string &request) {
	unsigned long crlf;
	if (request.find("POST") != request.npos) {
		unsigned long transfer_encoding = request.find("Transfer-Encoding: chunked");
		if (transfer_encoding == request.npos)
			return (true); // это не чанковый запрос, но нужна проверка на multipart??
		crlf = request.rfind("\r\n0\r\n\r\n");
		if ((crlf == request.npos) || (crlf != request.length() - 7))
			return (false); // последний chunk еще не пришел
		return (true); // последний chunk пришел
	}
	return (true); // это не чанковый запрос
}

void handle_message(int client, std::map<int, Server> &client_server, std::map<int, std::string> &client_request) {
	int bytes_sent;
	
	std::map<int, Server>::iterator cs_it = client_server.find(client);
	std::map<int, std::string>::iterator cr_it = client_request.find(client);
	bool check_chunked = check_if_chunked(cr_it->second);
	bool check_multipart = check_if_multipart(cr_it->second);
	if (check_multipart && check_chunked) { // пришел последний чанк или это был не чанковый запрос
		std::cout << "REQUEST " << cr_it->second << std::endl;
		response answer(cr_it->second, cs_it->second, client);
		bytes_sent = send(client, answer.get_char(), answer.get_len(), 0);
		// std::cout << "client " << client << " " << answer.get_char() << std::endl;
		// std::cout << bytes_sent << " ! " << answer.get_len() << std::endl;
		client_server.erase(cs_it);
		client_request.erase(cr_it);
		close(client);
		if (bytes_sent <= 0)
			perror("send error");
		return ;
	}
	bytes_sent = send(client, "HTTP/1.1 100 Continue\n\n", 23, 0); // это был не последний чанк
	if (bytes_sent <= 0)
		perror("send error");
	return ;
}

void get_message(int client, std::map<int, std::string> &client_request) {
	char b[1024] = {0};
	int receive;
	std::string request;
	while ((receive = recv(client, b, 1023, 0)) > 0) {
		b[receive] = 0;
		std::string r(b);
		request += r;
		if (receive < 1023)
			break ;
	}
    if (receive == 0) { // zero size of len mean the client closed connection
		close (client);
		return ;
	}
	else if (receive < 0) {
		perror ("recieve error");
		close (client);
		return ;
	}
	std::map<int, std::string>::iterator it = client_request.find(client);
	if (it == client_request.end())
		client_request.insert(std::pair<int, std::string>(client, request));
	else
		it->second += request;
	return ;
}

void main_loop(int kq, std::map<int, Server> &socket_server) {
	int new_socket;
	struct kevent event;
	struct sockaddr_in address;
    int addrlen = sizeof(address);
	std::map<int, Server> client_server;
	struct kevent for_read, for_write, events[10000];
	std::map<int, std::string> client_request;

    while(1) {
        int events_count = kevent(kq, NULL, 0, events, 10000, NULL);
        if (events_count < 0) {
            perror("kevent2 error");
			close_sockets(socket_server);
            exit(close_sockets(client_server));
        }
        for(int n = 0; n < events_count; ++n) {
			event = events[n];
			int client_fd = (int)event.ident;
			std::map<int, Server>::iterator connection = socket_server.find(client_fd);
			if (events[n].flags & EV_EOF)
				close (client_fd);
            else if (connection != socket_server.end()) {
                if ((new_socket = accept(connection->first, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                    perror("accept error");
                    close_sockets(socket_server);
            		exit(close_sockets(client_server));
                }
                // setup nonblocking socket
                fcntl(new_socket, F_SETFL, O_NONBLOCK);
				EV_SET(&for_read, new_socket, EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, 0);
				if (kevent(kq, &for_read, 1, NULL, 0, NULL) < 0) {
					perror("kevent3 error");
					close_sockets(socket_server);
            		exit(close_sockets(client_server));
				}
				client_server.insert(std::pair<int, Server>(new_socket, connection->second));
            }
            else if (event.filter == EVFILT_READ) { // IN event for others(new incoming message from client
				get_message(client_fd, client_request);
				EV_SET(&for_write, client_fd, EVFILT_WRITE, EV_ADD, 0, 0, 0);
				if (kevent(kq, &for_write, 1, NULL, 0, NULL) < 0) {
					perror("kevent5 error");
					close_sockets(socket_server);
            		exit(close_sockets(client_server));
				}
			}
			else if (event.filter == EVFILT_WRITE)//запись в дескриптор
				handle_message(client_fd, client_server, client_request);
        }
    }
	return ;
}

int main(int argc, char const *argv[])
{
	Server	**serv;
	Server	_serv;
	std::string config;
	if (argc == 1) {
		argc = 2;
		config = "config.conf";
	}
	else if (argc == 2)
		config = argv[1];
    else {
		std::cerr << "error number of arguments!" << std::endl;
        return (1);
	}
	ft_count_serv_and_local(config, &_serv);
	if (_serv.count_serv == 0) {
		std::cerr << "bad config!" << std::endl;
		exit (1);
	}
    serv = parser_conf(config, &_serv);
	if (!serv) {
		std::cerr << "bad config!" << std::endl;
		exit (1);
	}
	int common_count_of_ports = 0;
	_serv.count_serv = serv[0]->count_serv;
	for (int i = 0; i < _serv.count_serv; i++) {
		common_count_of_ports += serv[i]->port.size();
	}
	int server_fd[common_count_of_ports];
    struct sockaddr_in address[common_count_of_ports];
	int s = 0;
	struct in_addr inp;
	std::map<int, Server> socket_server;
	for (int i = 0; i < _serv.count_serv; i++) {
		for (unsigned long j = 0; j < serv[i]->port.size(); j++) {
			if ((server_fd[s] = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
				std::cerr << "In socket error" << std::endl;
				continue ;
			}
			fcntl(server_fd[s], F_SETFL, O_NONBLOCK);
			address[s].sin_family = AF_INET;
			if (inet_aton((const char*)(serv[i]->ip.c_str()), &inp) == 0) {
				close (server_fd[s]);
				std::cerr << "inet_aton error for ip " << serv[i]->ip << std::endl;
				continue ;
			}
			address[s].sin_addr.s_addr = inp.s_addr;
			address[s].sin_port = htons(serv[i]->port[j]);
			memset(address[s].sin_zero, '\0', sizeof address[s].sin_zero);

			// чтобы можно было не ждать освобождения порта
			int reuse = 1;
			if (setsockopt(server_fd[s], SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
				std::cerr << "setsockopt error" << std::endl;
			// 
			
			if (bind(server_fd[s], (struct sockaddr *)&address[s], sizeof(address[s])) < 0) {
				std::cerr << "bind error for ip " << serv[i]->ip << " port "<< serv[i]->port[j] << std::endl;
				close (server_fd[s]);
				continue ;
			}
			if (listen(server_fd[s], 10) < 0) {
				std::cerr << "listen error for ip " << serv[i]->ip << " port "<< serv[i]->port[j] << std::endl;
				close (server_fd[s]);
				continue ;
			}
			socket_server.insert(std::pair<int, Server>(server_fd[s], *serv[i]));
			s++;
		}
	}
    int kq = kqueue();
	if (kq == -1) {
		perror("kqueue error");
		exit (close_sockets(socket_server));
	}
	struct kevent ke[common_count_of_ports];
	for (int i = 0; i < common_count_of_ports; i++) {
		EV_SET(&ke[i], server_fd[i], EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
		if (kevent(kq, &ke[i], 1, NULL, 0, NULL) < 0) {
			perror("kevent1 error");
			exit (close_sockets(socket_server));
		}
	}
	main_loop(kq, socket_server);
	close_sockets(socket_server);
	for (int i = 0; i < serv[0]->count_serv; i++) {
		delete serv[i];
	}
	delete [] serv;
    return (0);
}
