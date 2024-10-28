#ifndef UTILS_H
# define UTILS_H

# include "../webserv.h"

std::vector<std::string>	tokenize(const std::string& str, const std::string& delim);

// some utils for the cgi script-parsing:
bool ends_with(const std::string& str, const std::string& suffix);
std::string clean_file_path(const std::string& url);
std::string get_query_string(const std::string& url);

// to be DELETED later:
struct ServerData;

void fill_server_data(std::vector<ServerData>& servers);
void print_server_data(const std::vector<ServerData>& servers);

#endif