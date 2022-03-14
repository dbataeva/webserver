#include "../includes/Pars_conf.hpp"

std::vector<std::string>	pars_method(std::string str_local) {
	int	i = 0;
	int	res = 0;
	std::vector<std::string>	vec_method;

	if ((res = str_local.find("GET")) != -1) {
        vec_method.push_back("GET");
		i++;
	}
	if ((res = str_local.find("POST")) != -1) {
		vec_method.push_back("POST");
		i++;
	}
	if ((res = str_local.find("DELETE")) != -1) {
		vec_method.push_back("DELETE");
		i++;
	}
	return (vec_method);
}

std::string	pars_root(std::string str_local) {
    std::string	sub_str;
    int			i = 0;
    int			j;

    while (str_local[i] == ' ' || str_local[i] == '\t')
        i++;
    if (str_local[i] == 'r') {
        i += 4;
        sub_str = &str_local[i];
    }	
    while (str_local[i] == ' ' || str_local[i] == '\t')
        i++;
    sub_str = &str_local[i];
    j = sub_str.length();
    while (sub_str[j] != ';') {
        j--;
    }
	return (sub_str.substr(0, j));
}

std::string	pars_index(std::string str_local) {
    std::string	sub_str;
    int			i = 0;
    int			j;

    while (str_local[i] == ' ' || str_local[i] == '\t')
        i++;
    if (str_local[i] == 'i') {
        i += 5;
        sub_str = &str_local[i];
    }	
    while (str_local[i] == ' ' || str_local[i] == '\t')
        i++;
    sub_str = &str_local[i];
    j = sub_str.length();
    while (sub_str[j] != ';') {
        j--;
    }
	return (sub_str.substr(0, j));
}

bool	pars_autoindex(std::string str_local, Location *location) {
    std::string	sub_str;
    int			i = 0;
    int			j;
    bool        res = false;

    while (str_local[i] == ' ' || str_local[i] == '\t')
        i++;
    if (str_local[i] == 'a') {
        i += 9;
        sub_str = &str_local[i];
    }	
    while (str_local[i] == ' ' || str_local[i] == '\t')
        i++;
    sub_str = &str_local[i];
    j = sub_str.length();
    while (sub_str[j] != ';')
        j--;
    if (sub_str.substr(0, j) == "off")
        res = false;
    else if (sub_str.substr(0, j) == "on")
        res = true;
    if (sub_str.substr(0, j) != "off" && sub_str.substr(0, j) != "on") {
        location->flag_er_auto = 1;
    }
	return (res);
}

bool	pars_authentication(std::string str_local) {
	std::string	sub_str;
    int			i = 0;
    int			j;
    bool        res = false;

    while (str_local[i] == ' ' || str_local[i] == '\t')
        i++;
    if (str_local[i] == 'a') {
        i += 14;
        sub_str = &str_local[i];
    }	
    while (str_local[i] == ' ' || str_local[i] == '\t')
        i++;
    sub_str = &str_local[i];
    j = sub_str.length();
    while (sub_str[j] != ';')
        j--;
    if (sub_str.substr(0, j) == "off")
        res = false;
    else if (sub_str.substr(0, j) == "on") {
        res = true;
    }
	return (res);
}

int pars_max_body(std::string str_local) {
	std::string	sub_str;
    int			i = 0;
    int			j;

    while (str_local[i] == ' ' || str_local[i] == '\t')
        i++;
    if (str_local[i] == 'm') {
        i += 8;
        sub_str = &str_local[i];
    }	
    while (str_local[i] == ' ' || str_local[i] == '\t')
        i++;
    sub_str = &str_local[i];
    j = sub_str.length();
    while (sub_str[j] != ';') {
        j--;
    }
	return (std::atoi(sub_str.substr(0, j).c_str()));
}

std::string pars_redirect(std::string str_local) {
    std::string	sub_str;
    int			i = 0;
    int			j;

    while (str_local[i] == ' ' || str_local[i] == '\t')
        i++;
    if (str_local[i] == 'r') {
        i += 8;
        sub_str = &str_local[i];
    }	
    while (str_local[i] == ' ' || str_local[i] == '\t')
        i++;
    sub_str = &str_local[i];
    j = sub_str.length();
    while (sub_str[j] != ';') {
        j--;
    }
	return (sub_str.substr(0, j));
}

std::string pars_cgi(std::string str_local) {
	std::string	sub_str;
    int			i = 0;
    int			j;

    while (str_local[i] == ' ' || str_local[i] == '\t')
        i++;
    if (str_local[i] == 'c') {
        i += 3;
        sub_str = &str_local[i];
    }	
    while (str_local[i] == ' ' || str_local[i] == '\t')
        i++;
    sub_str = &str_local[i];
    j = sub_str.length();
    while (sub_str[j] != ';') {
        j--;
    }
	return (sub_str.substr(0, j));
}

std::string pars_path(std::string str_local) {
    std::string     sub_str;
    int             i = 0;

    while (str_local[i] == ' ' || str_local[i] == '\t')
        i++;
    if (str_local[i] == 'l') {
        i += 8;
        sub_str = &str_local[i];
    }
    i = 0;
    while (sub_str[i] == ' ')
        i++;
    sub_str = &sub_str[i];
    i = 0;
    while (sub_str[i] != ' ')
        i++;
    sub_str = sub_str.substr(0, i);
    return (sub_str);
}

Location *pars_location(std::vector<std::string> str_local, Server *_serv) {
	Location	*location = new Location;
    int res = 0;
    int length = str_local.size();

    for (int i = 0; i < length; i++) {
        if ((res = str_local[i].find("root ")) != -1)
        	location->root = pars_root(str_local[i]);
    }
    if (location->root == "\0") {
        if (_serv->root_serv != "\0") {
            location->flag_er_auto = 1;
            location->root = _serv->root_serv;
        }   
        else
            location->flag_er = 1;
    }
    if (location->root != "\0") {
        for (int i = 0; i < length;) {
            if ((res = str_local[i].find("location ")) != -1)
                location->path = pars_path(str_local[i]);
            if ((res = str_local[i].find("method ")) != -1)
                location->method = pars_method(str_local[i]);
            if ((res = str_local[i].find("index ")) != -1 && (res = str_local[i].find("autoindex ")) == -1)
                location->index = pars_index(str_local[i]);
            if ((res = str_local[i].find("autoindex ")) != -1) {
                location->flag_er_auto = 0;
                location->autoindex = pars_autoindex(str_local[i], location);
            }
            if ((res = str_local[i].find("cgi ")) != -1)
                location->cgi = pars_cgi(str_local[i]);
            if ((res = str_local[i].find("authentication ")) != -1)
                location->authentication = pars_authentication(str_local[i]);
            if ((res = str_local[i].find("maxBody ")) != -1)
                location->max_body = pars_max_body(str_local[i]);
            if ((res = str_local[i].find("redirect ")) != -1)
            {
                location->redirect = pars_redirect(str_local[i]);
                location->flag_redirect = 1;
            }
                
            i++;
        }
    }
    if (location->flag_er_auto < 0) {
        location->flag_er_auto = 1;
    }
    
    if (_serv->flag_redirect_serv == 1) {
        location->redirect = _serv->redirect_serv;
        location->flag_redirect = 1;
    }
   
	return (location);
}