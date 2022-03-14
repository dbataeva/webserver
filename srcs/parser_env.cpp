#include "../includes/parser_env.hpp"

char** add_env(std::string s1)
{
    std::cout << s1 << std::endl;
    parser_env args;
    std::vector<std::string> v;
    std::map<std::string, std::string> map_env;
    std::string str;
    int pos = 0;
    int i = 0;
    if((pos = s1.find("\r\n\r\n", pos)) != -1){
        s1.erase(pos+4, s1.length());
    }
    pos = 0;
    for(std::string::iterator it = s1.begin(); it != s1.end(); it++)
    {
        if((pos = s1.find("\n", pos)) != -1)
        {
            for(; i != pos; i++)
            {
                if(*it != '\n')
                {
                    str += *it;
                    it++;
                }
            }
            pos++;
            v.push_back(str);
            str.clear();
        }
    }
    pos = 0;
    i = 0;
    std::string start = *v.begin();
    start += " ";
    for(size_t j = 0; j != start.length(); j++)
    {
        if((pos = start.find(" ", pos)) != -1)
        {
            std::string s2;
            for(; i != pos; i++)
            {
                if(start[i] != ' ')
                    s2 += start[i];
            }
            pos++;
            if (args.metod == "")
                args.metod = s2;
            else if (args.path_info == "")
            {
                int res = 0;
                if((res = s2.find("?", res)) != -1)
                {
                    res++;
                    for(size_t i = res;  i != s2.length(); i++)
                        args.data += s2[i];
                    res--;
                    s2.erase(res);
                    args.path_info = s2;
                }
                else
                    args.path_info = s2;
            }
            else if (args.protocol == "")
                args.protocol = s2;
            s2.clear();
        }
    }
    pos = 0;
    if((pos = s1.find("Cookie", pos)) != -1){
       for(int i = pos+8; s1[i] != '\n'; i++)
            args.cookie += s1[i];
    }
    map_env["HTTP_AUTH_TYPE"]			= "anonymous";
    map_env["HTTP_CONTENT_LENGTH"]		= "0";
    map_env["HTTP_CONTENT_TYPE"]		= "0";
    map_env["HTTP_GATEWAY_INTERFACE"]	= "CGI/1.1";
    map_env["HTTP_PATH_INFO"]			= args.path_info;
    map_env["HTTP_PATH_TRANSLATED"]		= args.path_info;
    map_env["HTTP_QUERY_STRING"]		= args.data;
    map_env["HTTP_REMOTE_ADDR"]			= args.ip;
    map_env["HTTP_REMOTE_IDENT"]		= args.ident;
    map_env["HTTP_REMOTE_USER"]			= args.user;
    map_env["HTTP_REQUEST_METHOD"]		= args.metod;
    map_env["HTTP_REQUEST_URI"]			= args.recv_uri;
    map_env["HTTP_SCRIPT_NAME"]			= args.script_name;
    map_env["HTTP_SERVER_NAME"]			= "localhost";
    map_env["HTTP_SERVER_PORT"]			= args.port;
    map_env["HTTP_SERVER_PROTOCOL"]		= args.protocol;
    map_env["HTTP_SERVER_SOFTWARE"]		= "webserver";
    map_env["HTTP_COOKIE"]              = args.cookie;
    pos = 0;
    i = 0;
    for(std::vector<std::string>::iterator it = v.begin(); it != v.end(); it++)
    {
        std::string headers = *it;
        i = 0;
        if((pos = headers.find(":")) != -1)
        {
            std::string s1 = "HTTP_";
            for(; i != pos; i++)
            {
                if(headers[i] != '\n')
                    s1 += toupper(headers[i]);
            }
            pos += 2;
            std::string s2;
            size_t i = pos;
            for(; i != headers.length(); i++)
            {
                if(headers[i] != '\n')
                    s2 += headers[i];
            }
            map_env[s1] = s2;
        }
    }
    char **array_env = new char *[map_env.size() + 1];
	array_env[map_env.size()] = 0; //???
	i = 0;
	for (std::map<std::string, std::string>::const_iterator it_array = map_env.begin(); it_array != map_env.end(); it_array++, i++)
			array_env[i] = strdup((it_array->first + "=" + it_array->second).c_str());
    return (array_env);
}

void get_body(std::string s1, std::string *body) {

    int pos = 0;
    std::string boundary;
    if((pos = s1.find("Transfer-Encoding: chunked", 0)) != -1){
       get_body_chunked(s1, body);
       return ;
    }
    pos = 0;
    pos = s1.find("\r\n\r\n", pos);
    pos += 4;
    while (s1[pos] != '\n')
    {
        boundary += s1[pos];
        pos++;
    }
    pos = 0;
    pos = s1.find("\r\n\r\n", pos);
    pos += 4;
    while (s1[pos] != '\0')
    {
        *body += s1[pos];
        pos++;
    }
    pos = 0;
    if((pos = s1.find("Content-Type: multipart/form-data; boundary=", 0)) != -1)
        body->replace(body->length() - boundary.length() - 3, body->length(), "\0");
    return ;
}

void get_body_chunked(std::string s1, std::string *body){
    int pos = 0;
    int res = 0;
    unsigned int x = 0;
    if((pos = s1.find("\r\n\r\n", pos)) != -1)
        pos += 4;
    s1.erase(0, pos);
    pos = 0;
    for(size_t j = 0; j != s1.length(); j++)
    {
        if((pos = s1.find("\r\n", pos)) != -1)
        {
            std::string n;
            n = s1.substr(res, pos);
            std::stringstream ss;
            ss << std::hex << n;
            ss >> x;
            if(x == 0)
                break;
            pos += 2;
            if((res = s1.find("\r\n", pos)) != -1){
                for(; pos != res; pos++)
                    *body += s1[pos];   
            }
            res += 2;
            j = res;
            pos = res;
        }
    }
    return ;
}