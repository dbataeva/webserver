#include <map>
#include <string>
#include <fstream>
#include <iostream>

std::map<std::string, std::string> save_types() {
	std::map<std::string, std::string> mime_types;
	std::fstream fs;

    fs.open("utils/mime.types", std::fstream::in);
	char line[80];
	fs.getline(line, 80);
	int i = 0;
	while (i < 108) {
		std::string str_line(line);
		size_t value_end = str_line.find(9);
		size_t key_begin = str_line.rfind(9);
		std::string value(str_line.begin(), str_line.begin() + value_end);
		std::string key(str_line.begin() + key_begin + 1, str_line.end());
		mime_types.insert(std::pair<std::string, std::string>(key, value));
		fs.getline(line, 80);
		i++;
	}
	fs.close();
	return (mime_types);
}
