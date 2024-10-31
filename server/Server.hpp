#ifndef SERVER_HPP
# define SERVER_HPP

# include "../webserv.h"
# include "Request.hpp"
// class Request;

# if defined(__APPLE__)
#  define RETOPT() setsockopt(_fd_server, IPPROTO_TCP, TCP_KEEPALIVE, &keep_idle, sizeof(keep_idle))
# else
#  define RETOPT() setsockopt(_fd_server, IPPROTO_TCP, TCP_KEEPIDLE, &keep_idle, sizeof(keep_idle))
# endif

class Server
{
	private:
		int							_fd_server;
		struct sockaddr_in			_address;
		const std::string			_name;
		const std::string			_index_file;
		const std::string			_data_dir;
		const std::string			_www_dir;
		bool						_directory_listing_enabled;
		size_t						_keepalive_timeout;
		size_t						_send_timeout;
		size_t						_max_body_size;
		std::vector<LocationData>	_locations;
		std::string					_CLF_line;

		// p. e. www/cgi-bin/hello_world.py
		std::string					_cgi_file_path;
		// p. e. QUERY_STRING= etc.
		std::vector<std::string>	_cgi_env_strings;
		// _cgi_post = _post_files (the first entry)
		std::string					_cgi_post;

		std::string			process_get(const Request& req);
		std::string			process_delete(const Request& req);
		std::string			process_post(const Request& req);
		std::string			process_cgi(const Request& req);
		std::string			extract_get_request(const std::string& request);

		size_t				find_matching_len(const std::string & a, const std::string & b);
		bool				is_method_allowed_at_location(const std::string & method, const LocationData & location);	
		void				find_longest_directory_match(const std::string & file_path, size_t & best_len, size_t & best_idx);
		std::string			redirect(const std::string& file_path, const std::string& method, bool & is_method_allowed);

		std::string			map_to_directory(const std::string& file_path);
		std::string			read_file(const std::string& file_path);
	public:
		Server(const std::string server_name, int port, const std::string ip_address, const std::string index_file,
		const std::string data_dir, const std::string www_dir, bool directory_listing_enabled, size_t keepalive_timeout,
		size_t send_timeout, size_t max_body_size, std::vector<LocationData> locations);
		~Server();

		int							getServerFD(void) const;
		struct sockaddr_in			getAddress(void) const;
		const std::string			getName(void) const;
		size_t						getMaxBodySize(void) const;
		std::string					getCgiFilePath(void) const;
		std::vector<std::string>	getCgiEnvStrings(void) const;
		std::string					getCgiPost(void) const;
		std::string					process_request(const Request& req);
		std::string					send_error_message(int error_code);
		void						log_CLF_line(void);
};

#endif /*SERVER_HPP*/