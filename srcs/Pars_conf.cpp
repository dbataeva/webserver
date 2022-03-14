#include "../includes/Pars_conf.hpp"

std::string         pars_path(std::string str_local);
std::vector<int>    pars_method(std::string str_local);
std::string	        pars_root(std::string str_local);
std::string	        pars_index(std::string str_local);
bool	            pars_autoindex(std::string str_local, Location location);
std::string	        pars_authentication(std::string str_local);
int	                pars_max_body(std::string str_local);
bool	            pars_redirect(std::string str_local);
std::string	        pars_cgi(std::string str_local);
Location            *pars_location(std::vector<std::string> str_local, Server *_serv);
int                 pars_root_serv(std::string str_in_conf, Server *serv);

void destroy(Server **ar_serv, int n) {  
    for (int i = n - 1; i >= 0; i--) 
    {
        ar_serv[i] = 0;
        delete ar_serv[i];
    }
    delete *ar_serv;
}

int		pars_listen(std::string str_in_conf, Server *serv) {
    int         i = 0;
    int         j = 0;
    std::string sub_str;

    while (str_in_conf[i] == ' ' || str_in_conf[i] == '\t')
        i++;
    if (str_in_conf[i] == 'l') {
        i += 6;
        sub_str = &str_in_conf[i];
    }
    while (str_in_conf[i] == ' ' || str_in_conf[i] == '\t')
        i++;
    sub_str = &str_in_conf[i];
    i = 0;
    if (sub_str[3] == '.') {
        
        serv->ip = sub_str.substr(0, 9);
        i += 10;
        if (sub_str[i-1] != ':') {
            serv->flag_error = 1;
            return (1);
        }
        sub_str = &sub_str[i];

        i = 0;
        while (sub_str[i] != ';') {
            if (sub_str[i] == ',') {
                    serv->port.push_back(atoi(sub_str.substr(j, i).c_str()));
                j = i + 1;
            }            
            i++;
        }
        if (sub_str[i] == ';')
            serv->port.push_back(atoi(sub_str.substr(j, i).c_str()));
    } else {
        i++;
        while (sub_str[i-1] != ':') {
            if (sub_str[i] == ',' || sub_str[i] == ':') {
                if (sub_str[i+1] == ':')
                    i++;
                serv->port.push_back(atoi(sub_str.substr(j, i).c_str()));
                j = i + 1;
            }
            i++;
        }
        sub_str = &sub_str[i];
        serv->ip = sub_str.substr(0, 9);
    }    
    return (0);
}

int		pars_error_page(std::string str_in_conf, Server *serv) {
    std::string	sub_str;
    int			i = 0;
    int			j;

    while (str_in_conf[i] == ' ' || str_in_conf[i] == '\t')
        i++;
    if (str_in_conf[i] == 'e') {
        i += 10;
        sub_str = &str_in_conf[i];
    }	
    while (str_in_conf[i] == ' ' || str_in_conf[i] == '\t')
        i++;
    sub_str = &str_in_conf[i];
    j = sub_str.length();
    while (sub_str[j] != ';')
        j--;
    serv->error_page = sub_str.substr(0, j);
    return (0);
}

int		pars_redirect_serv(std::string str_in_conf, Server *serv) {
    std::string	sub_str;
    int			i = 0;
    int			j;

    while (str_in_conf[i] == ' ' || str_in_conf[i] == '\t')
        i++;
    if (str_in_conf[i] == 'r') {
        i += 8;
        sub_str = &str_in_conf[i];
    }	
    while (str_in_conf[i] == ' ' || str_in_conf[i] == '\t')
        i++;
    sub_str = &str_in_conf[i];
    j = sub_str.length();
    while (sub_str[j] != ';')
        j--;
    serv->redirect_serv = sub_str.substr(0, j);
    return (0);
}

int		pars_server_name(std::string str_in_conf, Server *serv) {

    std::string	sub_str;
    int			i = 0;
    int			j;

    while (str_in_conf[i] == ' ' || str_in_conf[i] == '\t')
        i++;
    if (str_in_conf[i] == 's') {
        i += 11;
        sub_str = &str_in_conf[i];
    }	
    while (str_in_conf[i] == ' ' || str_in_conf[i] == '\t')
        i++;
    sub_str = &str_in_conf[i];
    j = sub_str.length();
    while (sub_str[j] != ';')
        j--;
    serv->server_name = sub_str.substr(0, j);
    return (0);
}

int     pars_root_serv(std::string str_in_conf, Server *serv) {
    std::string	sub_str;
    int			i = 0;
    int			j;

    while (str_in_conf[i] == ' ' || str_in_conf[i] == '\t')
        i++;
    if (str_in_conf[i] == 'r') {
        i += 4;
        sub_str = &str_in_conf[i];
    }	
    while (str_in_conf[i] == ' ' || str_in_conf[i] == '\t')
        i++;
    sub_str = &str_in_conf[i];
    j = sub_str.length();
    while (sub_str[j] != ';')
        j--;
    serv->root_serv = sub_str.substr(0, j);
    return (0);
}

void		ft_count_serv_and_local(std::string argv, Server *serv) {
    std::fstream	fileInput;
    std::string		str_in_conf;
    int				res = 0;

    fileInput.open(argv.c_str());
    if (!fileInput) {
        std::cout << "File opening error" << std::endl;
        exit(1);
    }
    if (fileInput.good())
    {
        while (getline(fileInput, argv).good()) {
            serv->str_conf.push_back(argv);
        }
        std::vector<std::string>::iterator  itBeg = serv->str_conf.begin();
        std::vector<std::string>::iterator  itEnd = serv->str_conf.end();
        while (itBeg != itEnd)
        {
            str_in_conf = *itBeg;
            if ((res = str_in_conf.find("server {")) != -1) {
                serv->count_serv += 1;
                serv->flag = 1;
            }
            if ((res = str_in_conf.find("location ")) != -1) {
                serv->count_local += 1;
            }
            str_in_conf.clear();
            itBeg++;
        }

        fileInput.close();
    }
}

std::vector<std::string>    conf_in_vector(std::string argv) {
    std::ifstream					fileInput;
    std::vector<std::string>		str_conf;
    long                            file_size;

    fileInput.open(argv.c_str());

    if (!fileInput) {
        std::cout << "File opening error" << std::endl;
        exit(1);
    }

    fileInput.seekg(0, std::ios::end);
    file_size = fileInput.tellg();
    if (file_size == 0)
    {
        std::cout << "The file is empty" << std::endl; 
        fileInput.close();
        str_conf.push_back("\0");
        return (str_conf);
    }
    else {
        fileInput.seekg(0, std::ios::beg);
        while (getline(fileInput, argv)) {
            str_conf.push_back(argv);
        }
    }

    return (str_conf);
}

Server    **check_duplicate_ports(Server **ar_serv) {
    int count_serv = ar_serv[0]->count_serv;
    int old_count_serv = ar_serv[0]->count_serv;
    
    for (int i = 0; i < count_serv; i++) {
        int count_port_in_serv = ar_serv[i]->port.size();
        for (int j = 0; j < count_port_in_serv - 1; j++) {
            if (ar_serv[i]->port[j] == ar_serv[i]->port[j + 1])
                ar_serv[i]->port[j + 1] = 0;         
        }
    }
    
    for (int i = 0; i < count_serv; i++) {
        int count_port_in_serv = ar_serv[i]->port.size();
        for (int j = 0; j < count_port_in_serv; j++) {
            std::vector<int>::iterator  itBeg = ar_serv[i]->port.begin();
            if (*(itBeg + j) == 0)
                ar_serv[i]->port.erase(itBeg + j);
            itBeg++;
        }
    }

    for (int i = 0; i < count_serv; i++) {
        int count_port_in_serv = ar_serv[i]->port.size();
        for (int j = 0; j < count_port_in_serv; j++) {
            int port = ar_serv[i]->port[j];
            int x = i + 1;
            while (x < count_serv) {
                int count_port_next_serv = ar_serv[x]->port.size();
                for (int y = 0; y < count_port_next_serv; y++) {
                    if (ar_serv[i]->ip ==  ar_serv[x]->ip) {
                        if (port == ar_serv[x]->port[y])
                            ar_serv[x]->port[y] = 0;
                    }
                }
                x++;
            }  
        }
    }

    for (int i = 0; i < count_serv; i++) {
        int count_port_in_serv = ar_serv[i]->port.size();
        for (int j = 0; j < count_port_in_serv; j++) {
            std::vector<int>::iterator  itBeg = ar_serv[i]->port.begin();
            if (*(itBeg + j) == 0)
                ar_serv[i]->port.erase(itBeg + j);
            itBeg++;
        }
    }
//-------------------
    for (int i = 0; i < count_serv; i++)
    {
        int port_size = ar_serv[i]->port.size();
        if (port_size == 0)
        {
            count_serv--;
        }
    }
    
    if (count_serv != 0)
    {
        if (old_count_serv != count_serv) {
            Server  **new_serv = new Server*[count_serv];
            int j = 0;
            for (int i = 0; i < old_count_serv;) {
                int port_size = ar_serv[i]->port.size();
                if (port_size == 0)
                    i++;
                else {
                    Server* _serv = new Server;
                    _serv = ar_serv[i];
                    _serv->count_serv = count_serv;
                    new_serv[j] = _serv;
                    i++;
                    j++;
                }
            }
            destroy(ar_serv, old_count_serv);
            delete[] ar_serv;
            return (new_serv);
        }
    }
    else
    {
        delete [] ar_serv;
        std::cout << "server error" << std::endl;
        exit(1);
    }
    
    return (ar_serv);
}



Server  **check_duplicate_path_in_local(Server **ar_serv) {
    int count_serv = ar_serv[0]->count_serv;
    int old_count_serv = ar_serv[0]->count_serv;

    for (int i = 0; i < count_serv; i++) {
        int count_local_in_serv = ar_serv[i]->locations.size();
        int x = 0;
        for (int j = 0; j < count_local_in_serv; j++) {
            x = j + 1;
            while (x < count_local_in_serv) {
                if (ar_serv[i]->locations[j]->path == ar_serv[i]->locations[x]->path) {
                    ar_serv[i]->path_matches += 1;
                }
                x++;
            }
        }
    }
    for (int i = 0; i < count_serv; i++) {
        
        if (ar_serv[i]->path_matches == 1) {
            count_serv--;
        }
    }
    if (count_serv != 0)
    {    
        if (old_count_serv != count_serv) {
                Server  **new_serv = new Server*[count_serv];
                for (int i = 0; i < old_count_serv;) {
                    for (int j = 0; j < count_serv; j++) {
                        if (ar_serv[i]->path_matches != 1) {
                            Server* _serv = new Server;
                            _serv = ar_serv[i];
                            _serv->count_serv = count_serv;
                            new_serv[j] = _serv;
                            i++;
                        }
                    }
                    i++;  
                }
                destroy(ar_serv, old_count_serv);
                delete [] ar_serv;
                return (new_serv);
        }
    }
    else
    {
        delete [] ar_serv;
        std::cout << "server error" << std::endl;
        exit(1);
    }
    
    return (ar_serv);
}

Server  **check_method_and_cgi(Server **ar_serv) {
    int len = ar_serv[0]->count_serv;

    for (int i = 0; i < len; i++) {
        int local_size = ar_serv[i]->locations.size();
        for (int j = 0; j < local_size; j++) {
            int method_size = ar_serv[i]->locations[j]->method.size();
            for (int x = 0; x < method_size; x++) {
                if ((ar_serv[i]->locations[j]->method[x] == "POST") || \
                (ar_serv[i]->locations[j]->method[x] == "DELETE")) {
                    if (ar_serv[i]->locations[j]->cgi == "\0") {
                        ar_serv[i]->locations[j]->no_cgi = 1;
                    }
                }
            }
        }
    }
    return (ar_serv);
}

Server  **check_autoindex(Server **ar_serv) {
    int count_serv = ar_serv[0]->count_serv;
    int old_count_serv = ar_serv[0]->count_serv;

    for (int i = 0; i < old_count_serv; i++) {
        int local_size = ar_serv[i]->locations.size();
        for (int j = 0; j < local_size; j++) {
            if (ar_serv[i]->locations[j]->flag_er_auto == 1) {
                    count_serv--;
                    break ;
            }
        }
    }
    if (count_serv != 0)
    {
        if (old_count_serv != count_serv) {
            std::cout << old_count_serv << std::endl;
            std::cout << count_serv << std::endl;
            
            Server  **new_serv = new Server*[count_serv];
            int x = 0;
            for (int i = 0; i < old_count_serv;) {
                int local_size = ar_serv[i]->locations.size();
                for (int j = 0; j < local_size; j++) {
                    if (ar_serv[i]->locations[j]->flag_er_auto != 1) {
                        Server* _serv = new Server;
                        _serv = ar_serv[i];
                        _serv->count_serv = count_serv;
                        new_serv[x] = _serv;
                        x++;
                        break ;
                    }
                    
                    
                }
                i++;
            }
            destroy(ar_serv, old_count_serv);
            delete [] ar_serv;
            return (new_serv);
        }
    }
    else 
    {
        delete [] ar_serv;
        std::cout << "server error" << std::endl;
        exit(1);
    }
    
    return (ar_serv);
}

Server  **check_location_and_redirect(Server **ar_serv) {
    int count_serv = ar_serv[0]->count_serv;
    int old_count_serv = ar_serv[0]->count_serv;

    for (int i = 0; i < count_serv; i++) {
        int local_size = ar_serv[i]->locations.size();
        for (int j = 0; j <= local_size; j++) {
            if (local_size == 0 && ar_serv[i]->redirect_serv.length() == 0) {
                count_serv--;
                for (int i = 0; i <= count_serv - 1; i++) {
                    if (i == 0 && count_serv - 1 == 0) {
                        Server  * _serv = new Server;
                        _serv->count_serv = 0;
                        ar_serv[i] = _serv;
                        return (ar_serv);
                    } else
                        ar_serv[i]->count_serv -= 1;
                }
                break ;
            }
        }
    }

    if (count_serv != 0) //here
    {
        if (old_count_serv != count_serv) {
            Server  **new_serv = new Server*[count_serv];
            for (int i = 0; i < old_count_serv;) {
                for (int j = 0; j <= count_serv;) {
                    int local_size = ar_serv[i]->locations.size();
                    if (local_size == 0 && ar_serv[i]->redirect_serv.length() == 0) {
                        i++;
                    } else {
                        Server* _serv = new Server;
                        _serv = ar_serv[i];
                        _serv->count_serv = count_serv;
                        new_serv[j] = _serv;
                        i++;
                        j++;
                    }
                    if (j == count_serv)
                        break ;
                }
                if (i == old_count_serv)
                    break ;
                i++;
            }
            destroy(ar_serv, old_count_serv);
            delete [] ar_serv;
            return (new_serv);
        }
    }
    else
    {
        delete [] ar_serv;
        std::cout << "server error" << std::endl;
        exit(1);
    }
    
    

    return (ar_serv);
}

Server **add_local_null(Server **ar_serv)
{
    int count_serv = ar_serv[0]->count_serv;
    Location    *_location = new Location;
    
    for (int i = 0; i < count_serv; i++)
    {
        // _location->path = "NULL";
        ar_serv[i]->locations.push_back(_location);
    }
    return (ar_serv);
}

Server		**parser_conf(std::string argv, Server *serv)
{
	int	i = 0;
	int	j = 0;
    int x = 0;
	size_t	res = 0;
    std::vector<std::string>	str_conf;
    str_conf = conf_in_vector(argv);
    // str_conf = serv->str_conf;
	int	length = str_conf.size();

    Server  **ar_serv = new Server*[serv->count_serv];

    if (length > 1)
    {
        for (; i < length;) {
            x = 0;
            if ((res = str_conf[i].find("server {")) != std::string::npos && i < length) {
                i++;
                Server* _serv = new Server;
                _serv->count_serv = serv->count_serv;
                ar_serv[j] = _serv;
                if (j == 0)
                    ar_serv[j]->serv_default = 1;
                j++;
                while ((res = str_conf[i].find("server {")) == std::string::npos && i < length) {
                    if ((res = str_conf[i].find("listen ")) != std::string::npos)
                        pars_listen(str_conf[i], _serv);
                    else if ((res = str_conf[i].find("error_page ")) != std::string::npos)
                        pars_error_page(str_conf[i], _serv);
                    else if ((res = str_conf[i].find("server_name ")) != std::string::npos)
                        pars_server_name(str_conf[i], _serv);
                    if ((res = str_conf[i].find("root ")) != std::string::npos)
                        pars_root_serv(str_conf[i], _serv);
                    else if ((res = str_conf[i].find("redirect ")) != std::string::npos)
                    {
                        pars_redirect_serv(str_conf[i], _serv);
                        _serv->flag_redirect_serv = 1;
                    }
                        
                    else if ((res = str_conf[i].find("location")) != std::string::npos) {
                        Location    *_location;
                        std::vector<std::string>    str_local;
                        _serv->flag_local = 1;
                        _serv->count_local++;
                        while (_serv->flag_local == 1) {
                            if ((res = str_conf[i].find("}") != std::string::npos))
                                _serv->flag_local = 0;
                            else {
                                str_local.push_back(str_conf[i]);
                                i++;
                            }
                        }
                        _location = pars_location(str_local, _serv);
                        if (_location->flag_er == 0) {
                            _serv->locations.push_back(_location);
                            _serv->flag_local_full = 1;
                        }
                        x++;
                    } 
                    else if (_serv->flag_redirect_serv == 1 && _serv->locations.size() == 0 && _serv->flag_local_full == 0) {
                        Location    *_location = new Location;
                        _serv->count_local++;
                        _location->path = "/";
                        _location->redirect = _serv->redirect_serv;
                        _serv->locations.push_back(_location);
                        _serv->locations[0]->flag_redirect = 1;
                    }
                    i++;
                    if (i == length)
                        break ;
                }	
            }
        }
        ar_serv = check_duplicate_path_in_local(ar_serv);
        ar_serv = check_autoindex(ar_serv);
        ar_serv = check_location_and_redirect(ar_serv);
        ar_serv = check_duplicate_ports(ar_serv);
        ar_serv = check_method_and_cgi(ar_serv);
        ar_serv = add_local_null(ar_serv);
    }

    // for (int i = 0; i < ar_serv[0]->count_serv; i++)
    // {
    //     std::cout << "ar_serv" << "[" << i << "].port.size(): " << ar_serv[i]->port.size() << std::endl;
    //     std::cout << "ip" << "[" << i << "]: " << ar_serv[i]->ip << std::endl;
    //     for (int j = 0; j < ar_serv[i]->port.size(); j++)
    //     {
    //         std::cout << "port" << "[" << j << "]: " << ar_serv[i]->port[j] << std::endl;
    //     }
    //     std::cout << "ar_serv" << "[" << i << "].error_page: " << ar_serv[i]->error_page << std::endl;
    //     std::cout << "ar_serv" << "[" << i << "].redirect_serv: " << ar_serv[i]->redirect_serv << std::endl;
    //     std::cout << "ar_serv" << "[" << i << "].server_name: " << ar_serv[i]->server_name << std::endl;
    //     for (int x = 0; x < ar_serv[i]->locations.size(); x++)
    //     {
    //         for (int m = 0; m < ar_serv[i]->locations[x]->method.size(); m++)
    //         {
    //             std::cout << "ar_serv" << "[" << i << "].location[" << x << "].method[" << m << "]: " << ar_serv[i]->locations[x]->method[m] << std::endl;
    //         }
    //         std::cout << "ar_serv" << "[" << i << "].location[" << x << "].path: " << ar_serv[i]->locations[x]->path << std::endl;
    //         std::cout << "ar_serv" << "[" << i << "].location[" << x << "].root: " << ar_serv[i]->locations[x]->root << std::endl;
    //         std::cout << "ar_serv" << "[" << i << "].location[" << x << "].index: " << ar_serv[i]->locations[x]->index << std::endl;
    //         std::cout << "ar_serv" << "[" << i << "].location[" << x << "].autoindex: " << ar_serv[i]->locations[x]->autoindex << std::endl;
    //         std::cout << "ar_serv" << "[" << i << "].location[" << x << "].cgi: " << ar_serv[i]->locations[x]->cgi << std::endl;
    //         std::cout << "ar_serv" << "[" << i << "].location[" << x << "].authentication: " << ar_serv[i]->locations[x]->authentication << std::endl;
    //         std::cout << "ar_serv" << "[" << i << "].location[" << x << "].redirect: " << ar_serv[i]->locations[x]->redirect << std::endl;
    //         std::cout << "ar_serv" << "[" << i << "].location[" << x << "].max_body: " << ar_serv[i]->locations[x]->max_body << std::endl;
    //         std::cout << "ar_serv" << "[" << i << "].location[" << x << "].FLAG_REDIRECT: " << ar_serv[i]->locations[x]->flag_redirect << std::endl;
    //     }
    //     std::cout << "ar_serv" << "[" << i << "].PATH_M: " << ar_serv[i]->path_matches << std::endl;
        
    //     std::cout << "----------------"<< "\n\n" << std::endl;
    // }

    return (ar_serv);
}