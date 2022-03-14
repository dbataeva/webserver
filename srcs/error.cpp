#include "../includes/error.hpp"

Error::Error() {

}

Error::Error(Error const &value) : body(value.body) {

}

Error::Error(std::string _number_error, std::string _error_page) {
	this->number_error = _number_error;
	this->error_page = _error_page;
	if(this->error_page != "/front/error.html")
		return ;	
	create_body(number_error);
}

Error   &Error::operator=(Error const &value)
{
    if (this != &value)
    {
        body = value.body;
    }
    return (*this);
}

Error::~Error(){}

void    Error::create_body(std::string number_error)
{
    body += "<!DOCTYPE html>\n";
    body += "<html>\n";
	body += "<head>\n";
	body += "\t<meta charset=\"UTF-8\">\n";
	body += "</head>\n";
	body += "<body>\n";
	body += "<style>\n";
	body += "\tbody {\n";
	body += "\t\tbackground: url(error_page/images/background.jpeg);\n";
	body += "\t}\n";
	body += "</style>\n";
	body += "<nav class=\"top-menu\">\n";
	body += "\t<ul class=\"menu-main\">\n";
	body += "\t\t<li class=\"right-item\"><a href=\"front/index.html\">Back to the main page</a></li>\n";
	body += "\t</ul>\n";
	body += "</nav>\n";
	body += "<link href=\"error_page/css/style_error_page.css\" rel=\"stylesheet\" type=\"text/css\">\n";
	body += "<br>\n";
	body += "<script>\n";
	body += "\tdocument.write (\"<h1 align=center>\"+\"OOPS number error: \"+\"";
	body += number_error;
	body += "\"+\"!!!\"+\"</h1>\");\n";
	body += "</script>\n";
	body += "<br><br>\n";
	body += "<script>\n";
	body += "\tvar a=Math.round(Math.random()*4)\n";
	body += "\tdocument.body.classList.add('center-pic-error');\n";
	body += "\timage = new Array();\n";
	body += "\timage[0]=\"error_page/images/error/error0.png\"\n";
	body += "\timage[1]=\"error_page/images/error/error1.png\"\n";
	body += "\timage[2]=\"error_page/images/error/error2.gif\"\n";
	body += "\timage[3]=\"error_page/images/error/error3.gif\"\n";
	body += "\timage[4]=\"error_page/images/error/error4.gif\"\n";
	body += "\tdocument.write (\"<img src=\"+image[a]+\">\");\n";
	body += "</script>\n";
    body += "</body>\n";
    body += "</html>";
}

std::string const   &Error::getBody(){
    return (body);
}