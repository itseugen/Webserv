#include "Server.hpp"

/* -------------------------------------------------------------------------- */
/*                           Orthodox Canonical Form                          */
/* -------------------------------------------------------------------------- */

/**
 * @brief Creates a webserver
 * @param server_name The name of the server
 * @param port The port the server should listen to
 * @param ip_address the IP adress of the server
 * @param index_file the index file of the server (relative to the www_dir)
 * @param data_dir the directory to POST/DELETE from (might be replaced by a redirect map in the future)
 * @param www_dir the location of the webpage files 
 * @param directory_listing_enabled true or false if the directory listing should be enabled
 * @param keepalive_timeout 
 * @param send_timeout 
 * @param max_body_size 
 */
Server::Server(const std::string server_name, int port, const std::string ip_address, const std::string index_file,
		const std::string data_dir, const std::string www_dir, bool directory_listing_enabled, size_t keepalive_timeout,
		size_t send_timeout, size_t max_body_size, std::vector<LocationData> locations)
	: _name(server_name), _index_file(index_file), _data_dir(data_dir), _www_dir(www_dir),
	_directory_listing_enabled(directory_listing_enabled), _keepalive_timeout(keepalive_timeout),
	_send_timeout(send_timeout), _max_body_size(max_body_size), _locations(locations)
{
	std::cout << "Server Default Constructor called" << std::endl;
	load_mime_types("mime_type.csv");
	_fd_server = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd_server == -1)
		throw SocketCreationFailedException(_name);
	int opt = 1;
	// Add keepalive here, not changed inside this branch since MK has to most current standart
	// Add send_out here
	if (setsockopt(_fd_server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		close(_fd_server);
		throw SetSocketOptionFailedException(_name);
	}
	_address.sin_family = AF_INET;
	if (port < 0 || port > 65535)
	{
		close(_fd_server);
		throw InvalidPortException(_name, port);
	}
	if (fcntl(_fd_server, F_SETFL, O_NONBLOCK) < 0)
	{
		close(_fd_server);
		throw SetSocketOptionFailedException(_name);
	}
	_address.sin_port = htons(port);
	// replace forbidden function, not done now since MKs branch has it
	if (inet_pton(AF_INET, ip_address.c_str(), &_address.sin_addr) != 1)
	{
		close(_fd_server);
		throw InvalidIPAdressException(_name, ip_address);
	}
	if (bind(_fd_server, (struct sockaddr *)&_address, sizeof(_address)) != 0)
	{
		close(_fd_server);
		throw BindFailedException(_name, ip_address);
	}
	if (listen(_fd_server ,SOMAXCONN) != 0)
	{
		close(_fd_server);
		throw ListenFailedException(_name);
	}

	int temp = _send_timeout + _keepalive_timeout + _directory_listing_enabled + _max_body_size;
	std::cout << temp << std::endl;
	std::cout << "Server is now listening on port " << port << std::endl;
}

Server::~Server()
{
	std::cout << "Server Default Destructor called" << std::endl;
	close(_fd_server);
}

/* -------------------------------------------------------------------------- */
/*                                  Functions                                 */
/* -------------------------------------------------------------------------- */

/// @brief Takes the http request and extracts the requested resource
/// @param request Takes the request sent by the client
/// @return String of the requested resource
std::string	Server::extract_get_request(const std::string& request)
{
	std::string::size_type	get = request.find("GET ") + 4;
	std::string::size_type	http = request.find(" HTTP/");

	if (get == std::string::npos || http == std::string::npos)
		return ("");
	return (request.substr(get, http - get));
}


/// @brief Compares 2 strings until the first differance
/// @param a string A
/// @param b string B
/// @return The Length where both strings matched
size_t	Server::find_matching_len(const std::string & a, const std::string & b)
{
	size_t l = std::min(a.size(), b.size());
	for (size_t i = 0; i < l; i++)
	{
		if (a[i] != b[i])
			return (i);
	}
	return (l);
}

/// @brief checks if request is allowed in a location
/// @param method method to used
/// @param location location where method should to be is used
/// @return if method can be used
bool	Server::is_method_allowed_at_location(const std::string & method, const LocationData & location)
{
	for (size_t i = 0; i < location.allowed_methods.size(); i++)
	{
		if (location.allowed_methods[i] == method)
			return (true);
	}
	return (false);
}

/// @brief Finds the location that matched the longest path
/// @param file_path path to redirect
/// @param best_len length that matched
/// @param best_idx index of location that matched
void Server::find_longest_directory_match(const std::string & file_path, size_t & best_len, size_t & best_idx)
{
	size_t	len;
	best_len = 0;
	best_idx = 0;
	for (size_t i = 0; i < _locations.size(); i++)
	{
		len = find_matching_len(_locations[i].path, file_path);
		if (len > best_len)
		{
			best_len = len;
			best_idx = i;
		}
	}
}

/// @brief redirects file_path based on the server locations
/// @param file_path path to redirect
/// @param method method used with this path
/// @param is_method_allowed indicates whether the method can be used at the redirection location
/// @return the redirected path
std::string	Server::redirect(const std::string& file_path, const std::string& method, bool & is_method_allowed)
{
	size_t	best_len;
	size_t	best_idx;
//	std::cout << "\n";
//	std::cout << "path '" << file_path << "'\n";
	find_longest_directory_match(file_path, best_len, best_idx);

	const LocationData & loc = _locations[best_idx];
//	std::cout << "path matched : '" << loc.path << "'\n";

	std::string non_redirected_path = file_path.substr(best_len);

	std::string redirected_path;
	if (loc.redirection)
	{
//		std::cout << "redirected   : '" << loc.path_to_redirect << "'\n";
		redirected_path = loc.path_to_redirect;
	}
	else
	{
//		std::cout << "rooted       : '" << _www_dir << "'\n";
		redirected_path = _www_dir;
	}

	if (non_redirected_path.size() != 0 && non_redirected_path[0] != '/' && redirected_path[redirected_path.size() - 1] != '/')
		redirected_path += "/";
	else if (non_redirected_path.size() != 0 && non_redirected_path[0] == '/' && redirected_path[redirected_path.size() - 1] == '/')
		redirected_path.pop_back();

//	std::cout << "non redirected : '" << non_redirected_path << "'\n";
//	std::cout << "redirected : '" << redirected_path << "'\n";

	if (non_redirected_path.size() != 0 && non_redirected_path[non_redirected_path.size() - 1] == '/')
		non_redirected_path += _index_file;

	is_method_allowed = is_method_allowed_at_location(method, loc);
//	if (!is_method_allowed)
//		std::cout << "Method '" << method << "' is not allowed at location '" << redirected_path << "'\n";

	return (redirected_path + non_redirected_path);

}

std::string	Server::get_mime_type(const std::string& file_path)
{
	std::string	file_extension = file_path.substr(file_path.find_last_of('.'));
	try
	{
		return (_mime_types.at(file_extension));
	}
	catch (const std::out_of_range& e)
	{
		return ("unknown/unknown");
	}
}

std::string	Server::read_file(const std::string& file_path)
{
	std::ifstream		file(file_path);
	std::stringstream	buffer;

	buffer << file.rdbuf();
	return (buffer.str());
}

void	Server::load_mime_types(const std::string& file_path)
{
	std::ifstream file(file_path);
	if (!file.is_open())
		throw OpenFailedException(_name, file_path);
	std::string	line;
	while(std::getline(file, line))
	{
		std::istringstream	ss(line);
		std::string			extension;
		std::string			mime_type;

		if (std::getline(ss, extension, '\t') && std::getline(ss, mime_type))
			_mime_types[extension] = mime_type;
	}
	file.close();
}

std::string	Server::process_request(const Request& req)
{
	int	method = req.get_method();

	switch (method)
	{
		case GET:
			return (process_get(req));
			break;
		case DELETE:
			return (process_delete(req));
			break;
		case POST:
			return (process_post(req));
			break;
	}
	return (send_error_message(400));
}

std::string	Server::process_get(const Request& req)
{
	std::string	url = req.get_file_path();
	bool is_method_allowed;
	std::string	file_path = redirect(url, "GET", is_method_allowed);
	if (!is_method_allowed)
	{
		//	METHOD CANNOT BE USED IN THE REDIRECTED DIRECTORY
		return (send_error_message(405));
	}
	std::string	response = "HTTP/1.1 200 OK\r\n";

	if (_directory_listing_enabled == true && std::filesystem::is_directory(file_path))
	{
		if (file_path != redirect("/" + _data_dir + "/", "", is_method_allowed) && file_path != redirect("/" + _data_dir, "", is_method_allowed))
		{
			return (send_error_message(403));
		}
		response += "Content-Type: text/html\r\n";
		std::string	response_body = "<!DOCTYPE html>\n<html>\n<head><title>Index of " + req.get_file_path() + "</title></head>\n";
		response_body += "<body>\n<hr>\n<ul>\n";
		response_body += "    <li><a href=\"../\">Parent Directory</a></li>\n";
		for (const auto& entry : std::filesystem::directory_iterator(file_path))
		{
			response_body += "    <li><a href=\"" + _data_dir + "/" + entry.path().filename().string() + "\">" + entry.path().filename().string() + "</a></li>\n";
		}
		response_body += "</ul>\n<hr>\n</body>\n</html>\n";
		response += "Content-Length: " + std::to_string(response_body.size()) + "\r\n";
		response += "\r\n";
		response += response_body;
		// if (send(req.get_fd(), response.c_str(), response.size(), 0) == -1)
		// 		throw SendFailedException(_name, req.get_fd());
		return (response);
	}
	else if (_directory_listing_enabled == false && std::filesystem::is_directory(file_path))
	{
		return (send_error_message(403));
	}
	else if (std::filesystem::exists(file_path))
	{
		std::string	file_content = read_file(file_path);
		std::string	mime_type = get_mime_type(file_path);
		response += "Content-Type: " + mime_type + "\r\n";
		response += "Content-Length: " + std::to_string(file_content.size()) + "\r\n";
		response += "\r\n";
		response += file_content;
		// if (send(req.get_fd(), response.c_str(), response.size(), 0) == -1)
		// 		throw SendFailedException(_name, req.get_fd());
		return (response);
	}
	else
		return (send_error_message(404));
}

std::string	Server::process_delete(const Request& req)
{
	std::string	url = req.get_file_path();
	//std::string	file_path = _www_dir + "/" + _data_dir + "/" + url;
	bool is_method_allowed;
	std::string	file_path = redirect(url, "DELETE", is_method_allowed);
	if (!is_method_allowed)
	{
		//	METHOD CANNOT BE USED IN THE REDIRECTED DIRECTORY
		return (send_error_message(405));
	}
	if (std::filesystem::exists(file_path))
	{
		if (std::remove(file_path.c_str()) == 0)
		{
			// std::string	response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
			// if (send(req.get_fd(), response.c_str(), response.size(), 0) == -1)
			// 	throw SendFailedException(_name, req.get_fd());
			return (send_error_message(200));
		}
		else
			return (send_error_message(500));
	}
	else
		return (send_error_message(404));
}

std::string	Server::process_post(const Request& req)
{
	long unsigned int	failed = 0;
	for (const auto& entry : req._post_files)
	{
		const std::string&	file_name = entry.first;
		const std::string&	content = entry.second;
		//std::string			full_path = _www_dir + "/" + _data_dir + "/" + file_name;
		bool is_method_allowed;
		std::string	full_path = redirect(file_name, "POST", is_method_allowed);
		if (!is_method_allowed)
		{
			//	METHOD CANNOT BE USED IN THE REDIRECTED DIRECTORY
			return (send_error_message(405));
		}

		std::ofstream out_file(full_path, std::ios::binary);
		if (!out_file)
		{
			std::cout << "Failed" << std::endl;
			failed++;
			continue;
		}
		out_file << content;
		out_file.close();
	}
	if (failed == req._post_files.size())
		return (send_error_message(500));
	else if (failed > 0)
		return (send_error_message(207));
	else
		return (send_error_message(200));
}


std::string	Server::send_error_message(int error_code)
{
	std::string	url = std::to_string(error_code);
	std::string	file_path = "error_pages/" + url + ".jpg";
	if (std::filesystem::exists(file_path))
	{
		std::string	file_content = read_file(file_path);
		std::string	mime_type = get_mime_type(file_path);
		std::string	response = "HTTP/1.1 " + std::to_string(error_code) + " Error\r\n";
		response += "Content-Type: " + mime_type + "\r\n";
		response += "Content-Length: " + std::to_string(file_content.size()) + "\r\n";
		response += "\r\n";
		response += file_content;
		// if (send(req.get_fd(), response.c_str(), response.size(), 0) == -1)
		// 		throw SendFailedException(_name, req.get_fd());
		return (response);
	}
	else
	{
		std::string response = "HTTP/1.1 404 Not Found\r\n";
		response += "Content-Type: text/html\r\n";
		response += "Content-Length: " + std::to_string(strlen("404 File Not Found")) + "\r\n";
		response += "\r\n"; // End of headers
		response += "404 File Not Found"; // Body
		// if (send(req.get_fd(), response.c_str(), response.size(), 0) == -1)
		// 	throw SendFailedException(_name, req.get_fd());
		return (response);
	}
	// return (0);
}

/* -------------------------------------------------------------------------- */
/*                                Getter/Setter                               */
/* -------------------------------------------------------------------------- */

int		Server::getServerFD(void) const
{
	return (_fd_server);
}

struct sockaddr_in	Server::getAddress(void) const
{
	return (_address);
}

const std::string	Server::getName(void) const
{
	return (_name);
}

size_t	Server::getMaxBodySize(void) const
{
	return (_max_body_size);
}
