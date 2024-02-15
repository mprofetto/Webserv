/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpListener.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 09:37:47 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/15 15:22:59 by mprofett         ###   ########.fr       */
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

TcpListener::~TcpListener()
{
	return;
}

TcpListener::TcpListener(const char * ipAdress, int port) :
	_ipAdress(ipAdress),
	_port(port)
{
	(void) this->_ipAdress;
	return;
}

void	TcpListener::bindSocket()
{
	sockaddr_in server;

	server.sin_family = AF_INET;
	server.sin_port = htons(this->_port);
	server.sin_addr.s_addr = INADDR_ANY;
	memset(&(server.sin_zero), 0, 8);
	if (bind(this->_socket, (sockaddr *)&server, sizeof(sockaddr)) < 0)
		throw socketBindingFailure();
}

void		TcpListener::init()
{
	int option_value = 1;

	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket < 0 || fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0)
		throw socketInitializationFailure();
	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value)) < 0)
		throw socketConfigurationFailure();
	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEPORT, &option_value, sizeof(option_value)) < 0)
		throw socketConfigurationFailure();
	bindSocket();
	if (listen(this->_socket, SOMAXCONN) < 0)
		throw socketListeningFailure();
	FD_ZERO(&this->_master_fd);
	FD_SET(this->_socket, &this->_master_fd);
}

void	send_test_reponse_to_client(int client_socket)
{
	std::string	hello("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");

	send(client_socket, hello.c_str(), hello.size(), 0);
}

void	TcpListener::handleNewConnection()
{
	std::cout << "Handling new connection\n";
	int			client_socket;
	sockaddr	client_addr;
	socklen_t	addr_size = sizeof(sockaddr);

	client_socket = accept(this->_socket, &client_addr, &addr_size);\
	if (client_socket < 0)
		throw socketAcceptingNewConnectionFailure();
	FD_SET(client_socket, &this->_master_fd);
}

void	TcpListener::handleNewMessage(int client_socket)
{
	std::cout << "Handling new message\n";
	char	buf[4096]; //limit should be fixed later
	int		bytesReceveid;

	memset(&buf, 0, 4096);
	bytesReceveid = recv(client_socket, buf, 4096, 0);
	if (bytesReceveid <= 0)
	{
		std::cout << "We closed " << client_socket << " connection\n";
		close(client_socket);
		FD_CLR(client_socket, &this->_master_fd);
		return;
	}
	std::string request(buf);
	std::cout << "Request is " << request << std::endl;
	send_test_reponse_to_client(client_socket);
}

void	TcpListener::run()
{
	bool	running = true;
	fd_set	ready_sockets;

	while (running)
	{
		ready_sockets = this->_master_fd;
		std::cout << "Before Select\n";
		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
			throw socketSelectFailure();
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &ready_sockets))
			{
				if (i == this->_socket)
				{
					std::cout << "i is: " << i << std::endl;
					handleNewConnection();
					std::cout << "New connection handled\n";
				}
				else
				{
					std::cout << "i is: " << i << std::endl;
					handleNewMessage(i);
				}
			}
		}
	}
}
