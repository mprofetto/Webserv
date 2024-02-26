/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:56:28 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/26 15:34:56 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../TcpListener.hpp"


void	TcpListener::runTcpListener()
{
	bool	running = true;
	fd_set	ready_to_read_fds;
	fd_set	ready_to_write_fds;
	Server	*server;

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
				server = this->getServerBySocket(i);
				if (server != NULL)
					handleNewConnection(server);
				else
					readRequest(i);
			}
			else if (FD_ISSET(i, &ready_to_write_fds))
				writeResponse(i, "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");
		}
	}
}

void	TcpListener::handleNewConnection(Server *server)
{
	std::cout << "Handling new connection\n";
	int			client_socket;
	sockaddr	client_addr;
	socklen_t	addr_size = sizeof(sockaddr);

	client_socket = accept(server->getSocket(), &client_addr, &addr_size);
	if (client_socket < 0)
		throw socketAcceptingNewConnectionFailure();
	FD_SET(client_socket, &this->_read_master_fd);
}

void	TcpListener::readRequest(int client_socket)
{
	std::cout << "Reading new request\n";
	std::string	raw_request;
	Server		*server;

	char	buf[this->_buffer_max];
	int		bytesReceveid;

	memset(&buf, 0, this->_buffer_max);
	bytesReceveid = recv(client_socket, buf, this->_buffer_max, 0);
	if (bytesReceveid <= 0)
	{
		close(client_socket);
		FD_CLR(client_socket, &this->_read_master_fd);
		return;
	}
	raw_request = buf;
	server = getServerBySocket(client_socket);
	//Request = parseRequest(std::string raw_request, Server *server);
	//handleRequest(Request request);
	//store reponse from request handler in map<fd, response> for response sending
	FD_SET(client_socket, &this->_write_master_fd);
}

void	TcpListener::writeResponse(int client_socket, std::string response)
{
	std::cout << "Sending response\n";
	send(client_socket, response.c_str(), response.size(), 0);
	FD_CLR(client_socket, &this->_write_master_fd);
}
