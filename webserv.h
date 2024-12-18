#ifndef WEBSERV_H
# define WEBSERV_H

# include <iostream>
# include <string>
# include <vector>
# include <deque>
# include <algorithm>
# include <limits>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/wait.h>
# include <netinet/in.h>
# include <unistd.h>
# include <fcntl.h>
# include <climits>
# include <sstream>
# include <cstring>
# include <poll.h>
# include <csignal>
# include <arpa/inet.h>
# include <exception>
# include <fstream>
# include <unordered_map>
# include <cstdio>
# include <filesystem>
# include <fcntl.h>
# include <chrono>

# ifdef __linux__
	#include <netinet/tcp.h>
# elif defined(__APPLE__)
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h> // This includes TCP_KEEPALIVE for macOS
# endif

# include "exceptions/Exceptions.hpp"
# include "utils/utils.h"
# include "parser/parsing/parse_config_file.hpp"
# include "log/Logger.hpp"
# include "server/MimeTypes.hpp"

// some output formatting macros:
# define BOLD(text) "\033[1m" << text << "\033[0m"
# define UNDERLINE(text) "\033[4m" << text << "\033[0m"
# define BOLD_UNDERLINE(text) "\033[1;4m" << text << "\033[0m"
# define GRAY(text) "\033[90m" << text << "\033[0m"
# define RED(text) "\033[31m" << text << "\033[0m"
# define CYAN(text) "\033[1;96m" << text << "\033[0m"
# define GREEN(text) "\033[32m" << text << "\033[0m"
# define YELLOW(text) "\033[33m" << text << "\033[0m"
# define ORANGE(text) "\033[38;5;214m" << text << "\033[0m"
# define ERROR(text) "\033[31mError: \033[0m" << text

#endif /*WEBSERV_H*/