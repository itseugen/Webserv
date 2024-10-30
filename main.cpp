#include "webserv.h"
#include "server/SocketManager.hpp"

/*
* Use curl localhost:PORT (replace port with actual value) to test responses
* USE wrk -t12 -c400 -d30s http://localhost:8080 to stress test server
* -t12 stands for 12 threads
* -c400 stands for 400 connections
* -d30s stands for a 30s test lenght
*/

void signal_handler(int signum);

int main(int argc, char **argv)
{
	std::cout << GREEN("🚀 Launching Webserv ... 🚀") << std::endl;
	if (argc != 2)
	{
		std::cout << ERROR("Expected usage: ") << UNDERLINE("./webserv") << " or " << UNDERLINE("./webserv <path_to_config_file>") << std::endl;
		return 0;
	}

	//receiving values from the `example1.config`:
	// fill_server_data(servers); //what the parser from the config will fill out
	// print_server_data(servers); // for verification
	// (void)argv;
	

	try
	{
		SocketManager	socket_manager;
		try
		{
			std::vector<ServerData> server_vec = read_config_file(argv[1]);
			for (size_t s = 0; s < server_vec.size(); s++)
			{
				ServerData const & server = server_vec[s];
				socket_manager.add_server(server.port_to_listen, std::make_unique<Server>(server.server_name, server.port_to_listen, "0.0.0.0", server.index_file,
				"usrimg", server.root, server.directory_listing, server.keepalive_timeout, server.send_timeout, server.max_request_size, server.locations));

				//	redirect testing, requires redirect() to be public
				//Server serv = Server(server.server_name, server.port_to_listen, "0.0.0.0", server.index_file,
				//"usrimg", server.root, server.directory_listing, server.keepalive_timeout, server.send_timeout, server.max_request_size, server.locations);
				//std::string strI, strO;
				//bool is_method_allowed;
				//strI = "/"; strO = serv.redirect(strI, "GET", is_method_allowed); std::cout << "'" << strI << "' mapped to '" << strO << "' " << is_method_allowed << "\n";
				//strI = "/test"; strO = serv.redirect(strI, "GET", is_method_allowed); std::cout << "'" << strI << "' mapped to '" << strO << "' " << is_method_allowed << "\n";
				//strI = "/test/"; strO = serv.redirect(strI, "GET", is_method_allowed); std::cout << "'" << strI << "' mapped to '" << strO << "' " << is_method_allowed << "\n";
				//strI = "/dir/test"; strO = serv.redirect(strI, "GET", is_method_allowed); std::cout << "'" << strI << "' mapped to '" << strO << "' " << is_method_allowed << "\n";
				//strI = "/42heilbronn"; strO = serv.redirect(strI, "GET", is_method_allowed); std::cout << "'" << strI << "' mapped to '" << strO << "' " << is_method_allowed << "\n";
				//strI = "/42heilbronn/test"; strO = serv.redirect(strI, "GET", is_method_allowed); std::cout << "'" << strI << "' mapped to '" << strO << "' " << is_method_allowed << "\n";
				//strI = "/google"; strO = serv.redirect(strI, "GET", is_method_allowed); std::cout << "'" << strI << "' mapped to '" << strO << "' " << is_method_allowed << "\n";
				//strI = "/google/test"; strO = serv.redirect(strI, "GET", is_method_allowed); std::cout << "'" << strI << "' mapped to '" << strO << "' " << is_method_allowed << "\n";
				//strI = "/uploads"; strO = serv.redirect(strI, "GET", is_method_allowed); std::cout << "'" << strI << "' mapped to '" << strO << "' " << is_method_allowed << "\n";
				//strI = "/uploads/test"; strO = serv.redirect(strI, "GET", is_method_allowed); std::cout << "'" << strI << "' mapped to '" << strO << "' " << is_method_allowed << "\n";
			}
		}
		catch(const std::exception& e)
		{
			std::cerr << "Exception: " << e.what() << std::endl;
		}
		// socket_manager.add_server(8080, std::make_unique<Server>("Instalight", 8080, "0.0.0.0", "index.html", "usrimg", "www_image_webpage", true, 0, 0, 3000000));
		// socket_manager.add_server(8081, std::make_unique<Server>("A little webserver", 8081, "0.0.0.0", "index.html", "usrimg", "www", true, 0, 0, 3000000));
		socket_manager.handle_requests();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	return (0);
}


