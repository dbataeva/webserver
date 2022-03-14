#include <map>
#include <string>
#include <fstream>
#include <iostream>

std::map<std::string, std::string> save_responses() {
	std::map<std::string, std::string> responses_status_codes;
	std::fstream fs;

    fs.open("utils/HTTP_response_status_codes", std::fstream::in);
	char line[40];
	fs.getline(line, 40);
	int i = 0;
	while (i < 69) {
		std::string str_line(line);
		size_t key_end = str_line.find(' ');
		std::string key(str_line.begin(), str_line.begin() + key_end);
		std::string value(str_line.begin() + key_end + 1, str_line.end());
		responses_status_codes.insert(std::pair<std::string, std::string>(key, value));
		fs.getline(line, 80);
		i++;
	}
	fs.close();
	return (responses_status_codes);
}