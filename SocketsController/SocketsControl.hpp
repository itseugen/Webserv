#ifndef SOCKETS_CONTROL_HPP
# define SOCKETS_CONTROL_HPP

# include "../webserv.h"

struct ServerData;

class SocketsControl
{
	private:
		std::vector<ServerData> 	_servers;
		std::vector<int>			_server_fds; //to keep track of the active server sockets
		std::vector<int>			_client_fds; //to keep track of the active client sockets
		std::vector<struct pollfd>	_poll_fds;
		std::vector<int>			_used_ports;

		SocketsControl(); //only instantiated with the vector of ServerData passed

	public:
		SocketsControl(const std::vector<ServerData> & serversData);
		~SocketsControl();

		void initServerSockets();
		void bind_socket_and_listen_to_port(std::vector<ServerData>::iterator server);
		void server_poll_data(std::vector<ServerData>::iterator server);
		void loop_for_connections();
		void accept_new_client_connection(int server_fd);
		void read_client_request(pollfd & client_poll_fd);

		void close_server_sockets();//not used yet

	
};

void signal_handler(int signum);



#endif //SOCKETS_CONTROL_HPP