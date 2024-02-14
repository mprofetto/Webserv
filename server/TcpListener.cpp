/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpListener.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 09:37:47 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/14 13:28:56 by mprofett         ###   ########.fr       */
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
	int option_value = 0;

	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket < 0 || fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0)
		throw socketInitializationFailure();
	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value)) < 0)
		throw socketConfigurationFailure();
	bindSocket();
	if (listen(this->_socket, SOMAXCONN) < 0)
		throw socketListeningFailure();
	FD_ZERO(&this->_master_fd);
	FD_SET(this->_socket, &this->_master_fd);
}

void	TcpListener::handleNewConnection()
{
	int	client_socket;

	client_socket = accept(this->_socket, nullptr, nullptr);
	if (client_socket < 0)
		throw socketAcceptingNewConnectionFailure();
	FD_SET(client_socket, &this->_master_fd);
}

void	TcpListener::handleNewMessage(int socket)
{
	char	buf[4096];
	int		bytesReceveid;

	memset(&buf, 0, 4096);
	bytesReceveid = recv(socket, buf, 4096, 0);
	if (bytesReceveid <= 0)
	{
		close(socket);
		FD_CLR(socket, &this->_master_fd);
	}
	else
	{

	}
}

void	TcpListener::run()
{
	bool	running = true;
	fd_set	master_fd;
	int		socketCount;
	int		temp_sock;

	while (running)
	{
		master_fd = this->_master_fd;
		socketCount = select(0, &master_fd, nullptr, nullptr, nullptr);
		for (int i = 0; i < socketCount; i++)
		{
			temp_sock = master_fd.fds_bits[i];
			if (temp_sock == this->_socket)
				handleNewConnection();
			else
				handleNewMessage(temp_sock);
		}
	}
}
