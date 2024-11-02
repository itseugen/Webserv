#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include "../stringing/StringArr.hpp"
#include "../stringing/StringDataTracker.hpp"

extern StringDataTracker	tracker;

/*
	these are for gather all the configuration data
	and setting default values
*/
class ConfigData
{
	public:
		class	MemberData
		{
			public:
				const std::string	parentName;
				const int			parentLine;
				const std::string	name;
				int					setAtLine;
				bool				isSet;
				StringArr			*data;

				MemberData(std::string pName, int pLine, std::string name);
				~MemberData();

				void	set(int argc, std::string args[], int line, std::string funcName, std::string className);
				std::string		get_string(std::string def) const;
				size_t			get_size_t(size_t def) const;
				unsigned short	get_ushort(unsigned short def) const;
				bool			get_bool(bool def) const;
				std::string		try_get_string(std::string def, bool & got) const;
				void			get_string_vec(std::vector<std::string> & vec) const;
				void	print(std::string tab);
		};

		static void	printFuncArgs(std::string func, int argc, std::string args[]);

		class ServerLocationData
		{
				static const std::string	className;
			public:
				const std::string	path;
				MemberData			allowed_methods;
				MemberData			redirection;

				ServerLocationData(int line, std::string path);
				~ServerLocationData();

				static void	set_allowed_methods(void * ptr, int line, int argc, std::string args[]);
				static void	set_redirection(void * ptr, int line, int argc, std::string args[]);

				void	print();
		};
		class ServerData
		{
				static const std::string	className;
			public:
				MemberData	server_name;
				MemberData	listen;
				MemberData	root;
				MemberData	index;
				MemberData	keepalive_timeout;
				MemberData	send_timeout;
				MemberData	max_body_size;
				MemberData	directory_listing;
				std::vector<ServerLocationData *> location;

				ServerData(int line);
				~ServerData();

				static void	set_server_name(void * ptr, int line, int argc, std::string args[]);
				static void	set_listen(void * ptr, int line, int argc, std::string args[]);
				static void	set_root(void * ptr, int line, int argc, std::string args[]);
				static void	set_index(void * ptr, int line, int argc, std::string args[]);
				static void	set_keepalive_timeout(void * ptr, int line, int argc, std::string args[]);
				static void	set_send_timeout(void * ptr, int line, int argc, std::string args[]);
				static void	set_max_body_size(void * ptr, int line, int argc, std::string args[]);
				static void	set_directory_listing(void * ptr, int line, int argc, std::string args[]);
				static void	*newLocation(void * ptr, int line, int argc, std::string args[]);

				void	print();
		};
		class HttpData
		{
				static const std::string	className;
			public:
				std::vector<ServerData *> server;
				MemberData	server_timeout_time;

				HttpData(int line);
				~HttpData();

				static void	*newServer(void * ptr, int line, int argc, std::string args[]);
				static void	set_server_timeout_time(void * ptr, int line, int argc, std::string args[]);

				void	print();
		};
		class MainData
		{
				static const std::string	className;
			public:
				std::vector<HttpData *> http;

				MainData();
				~MainData();

				static void	*newHttp(void * ptr, int line, int argc, std::string args[]);

				void	print();
		};
};
