#include "../includes/Autoindex.hpp"

Autoindex::Autoindex(std::string &root, std::string &path): body(std::string()), content_dir(std::vector<std::string>())
{   
    std::string cmd("cd ");
    cmd += root;
    cmd += " && /bin/ls -lah";
    FILE *proc = popen(cmd.c_str(),"r");
    char buf[1024];
    if(proc)
    {
        while (!feof(proc) && fgets(buf,sizeof(buf),proc))
        { 
            std::string s2;
            std::string s1(buf);
            int pos = 0;
            int count = 0;
			std::cout << s1 << std::endl;
            if((pos = s1.find("student", 0)) != -1){
                s1.erase(0, pos);
                if((pos = s1.find(' ', 0)) != -1){
                    while(s1[pos] == ' ')
                        pos++;
                    s1.erase(0, pos);
                    pos = 0;
                    std::string::iterator it = s1.begin();
                    while(it != s1.end()){
                        if(*it == ' '){
                            count++;
                            if(count == 5){
                                pos++;
                                it++;
                                break;
                            }
                        }
                        it++;
                        pos++;
                    }
                    s2 = s1;
                    s1.erase(it, s1.end());
                    s2.erase(0, pos);
                    data.push_back(s1);
                    content_dir.push_back(s2);
                }
            }
        }
    }
    else
        std::cout << "error" << std::endl;
    content_dir.erase(content_dir.begin());
    data.erase(data.begin());
    pclose(proc);
    create_body(path);
}

Autoindex::Autoindex(Autoindex const &value) : body(value.body), content_dir(value.content_dir){}

Autoindex   &Autoindex::operator=(Autoindex const &value)
{
    if (this != &value)
    {
        body = value.body;
        content_dir = value.content_dir;
    }
    return (*this);
}

Autoindex::~Autoindex(){}

std::string const   &Autoindex::getBody(){
    return (body);
}

void    Autoindex::create_body(std::string &path)
{
    body += "<!DOCTYPE html>\n";
    body += "<html lang=\"en\">\n";
    body += "<head>\n";
	body += "\t<link rel=\"icon\" href=\"data:image;base64,=\">\n";
    body += "\t<meta charset=\"UTF-8\">\n";
    body += "\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
    body += "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    body += "\t<title>Autoindex</title>\n";
    if(path.empty())
        body += "\t<h1>Index of &nbsp /";
    else
        body += "\t<h1>Index of &nbsp";
    body += path;
    body += "</h1>\n";
    body += "</head>\n";
    body += "<table align=\"left\">";
    std::vector<std::string>::iterator it_dir = content_dir.begin();
    std::vector<std::string>::iterator it_data = data.begin();
    for (; it_dir != content_dir.end() || it_data != data.end(); it_dir++, it_data++)
    {
        body += "\t<tr align=\"left\">";
        body += "<td align=\"left\" width=\"150\">";
        body += *it_data;
        body += "</td>";
        body += "<td width=\"200\">";
        body += "\t<a class=\"color_autoindex\" href=\"";
        body += path;
        body += "/";
        body += *it_dir;
        body += "\">";
        body += *it_dir;
        body += "</a>\n";
        body += "</td>";
        body += "\t</tr>\n";
    }
    body += "</table>\n";
    body += "</body>\n";
    body += "</html>";
}