#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../webserv.h"

# ifndef GET
#  define GET 0
# endif /*GET*/

# ifndef POST
#  define POST 1
# endif /*POST*/

# ifndef DELETE
#  define DELETE 1
# endif /*DELETE*/

/// @brief Takes the request from a client and parses it.
class Request
{
	private:
		int			method; // Assign one of the three defines above
		std::string	file_path;
	public:
		Request(const std::string& request);
		~Request();
		Request(const Request& copy);
		Request& operator=(const Request& copy);
};

#endif /*REQUEST_HPP*/