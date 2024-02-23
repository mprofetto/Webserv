/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpListener.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 09:37:47 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/23 14:49:11 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "TcpListener.hpp"

const char	*TcpListener::socketInitializationFailure::what(void) const throw()
{
	return ("Socket Initialization Failure");
}

const char	*TcpListener::socketConfigurationFailure::what(void) const throw()
{
	return ("Socket Configuration Failure");
}

const char	*TcpListener::socketBindingFailure::what(void) const throw()
{
	return ("Socket Binding Failure");
}

const char	*TcpListener::socketListeningFailure::what(void) const throw()
{
	return ("Socket Binding Failure");
}

const char	*TcpListener::socketSelectFailure::what(void) const throw()
{
	return ("Socket Select Failure");
}

const char	*TcpListener::socketAcceptingNewConnectionFailure::what(void) const throw()
{
	return ("Socket Accepting New Connection Failure");
}

const char	*TcpListener::socketPortIdentificationFailure::what(void) const throw()
{
	return ("Socket Port identification Failure");
}

TcpListener::~TcpListener()
{
	clearList(this->_servers);
	return;
}

TcpListener::TcpListener(std::string configfile) : _buffer_max(MAXBUFFERSIZE)
{
	parseConfigurationFile(configfile);
	return;
}

TcpListener::TcpListener(const char * ipAddress, int port, int buffer_max) :
	_ipAddress(ipAddress),
	_port(port),
	_buffer_max(buffer_max)
{
	return;
}

void	TcpListener::bindSocket(int port)
{
	sockaddr_in server;

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;
	memset(&(server.sin_zero), 0, 8);
	if (bind(this->_socket, (sockaddr *)&server, sizeof(sockaddr)) < 0)
		throw socketBindingFailure();
}

int		TcpListener::getPortFromSocket(int *socket)
{
	int			port;
	sockaddr_in	addr;
	socklen_t	addr_len = sizeof(sockaddr);

	if (getsockname(*socket, (sockaddr *)&addr, &addr_len) < 0)
		throw socketPortIdentificationFailure();
	port = htons(addr.sin_port);
	return (port);
}

void		TcpListener::init()
{
	// std::list<Server *>::const_iterator	it = this->_servers.begin();
	// Server								*server;
	int 									option_value = 1;

	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket < 0 || fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0)
		throw socketInitializationFailure();
	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value)) < 0)
		throw socketConfigurationFailure();
	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEPORT, &option_value, sizeof(option_value)) < 0)
		throw socketConfigurationFailure();
	bindSocket(this->_port);
	// while(it != this->_servers.end())
	// {
	// 	server = *it;
		// bindSocket(server->getPort());
	// 	it++;
	// }
	if (listen(this->_socket, SOMAXCONN) < 0)
		throw socketListeningFailure();
	FD_ZERO(&this->_read_master_fd);
	FD_ZERO(&this->_write_master_fd);
	FD_SET(this->_socket, &this->_read_master_fd);
}

void	send_test_reponse_to_client(int client_socket)
{
	std::string	hello("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");

	send(client_socket, hello.c_str(), hello.size(), 0);
}

void	TcpListener::handleNewConnection()
{
	std::cout << "handling new connection\n";
	int			client_socket;
	sockaddr	client_addr;
	socklen_t	addr_size = sizeof(sockaddr);

	client_socket = accept(this->_socket, &client_addr, &addr_size);
	if (client_socket < 0)
		throw socketAcceptingNewConnectionFailure();
	FD_SET(client_socket, &this->_read_master_fd);
}

void	TcpListener::readRequest(int client_socket)
{
	std::cout << "reading new request\n";
	char	buf[this->_buffer_max];
	int		bytesReceveid;

	memset(&buf, 0, this->_buffer_max);
	bytesReceveid = recv(client_socket, buf, this->_buffer_max, 0);
	if (bytesReceveid <= 0)
	{
		std::cout << "We closed " << client_socket << " connection\n";
		close(client_socket);
		FD_CLR(client_socket, &this->_read_master_fd);
		return;
	}
	std::string request(buf);
	std::cout << "Request is:\n" << request << std::endl;
	std::cout << "Port is " << getPortFromSocket(&client_socket) << std::endl;
	//give request to request parse
	//get result from request parser and give it to request handler
	//store reponse from request handler in map<fd, response> for response sending
	FD_SET(client_socket, &this->_write_master_fd);
}

void	TcpListener::writeResponse(int client_socket)
{
	std::cout << "sending response\n";
	std::string	hello("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");

	send(client_socket, hello.c_str(), hello.size(), 0);
	FD_CLR(client_socket, &this->_write_master_fd);
}

void	TcpListener::run()
{
	bool	running = true;
	fd_set	ready_to_read_fds;
	fd_set	ready_to_write_fds;

	while (running)
	{
		ready_to_read_fds = this->_read_master_fd;
		ready_to_write_fds = this->_write_master_fd;
		if (select(FD_SETSIZE, &ready_to_read_fds, &ready_to_write_fds, NULL, NULL) < 0)
			throw socketSelectFailure();
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &ready_to_read_fds))
			{
				if (i == this->_socket)
					handleNewConnection();
				else
					readRequest(i);
			}
			else if (FD_ISSET(i, &ready_to_write_fds))
				writeResponse(i);
		}
	}
}

void					TcpListener::printServers(void) const
{
	std::list<Server *>::const_iterator	it;
	const Server *	serv;

	it = this->_servers.begin();
	while (it != this->_servers.end())
	{
		serv = *it;
		serv->printDatas();
		std::cout << "\n";
		it++;
	}
}
