/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:56:28 by mprofett          #+#    #+#             */
/*   Updated: 2024/03/26 17:22:04 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../TcpListener.hpp"
# include "../../request_manager/includes/Cgi.hpp"

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
				if (server)
					handleNewConnection(server);
				else
				{
					readRequest(i);
					if (FD_ISSET(i, &_write_master_fd))
						handleRequest(i);
				}
			}
			else if (FD_ISSET(i, &ready_to_write_fds))
				writeResponse(i);
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
	std::string					raw_request;
	char						buffer[this->_buffer_max];
	int							bytesReceveid;

	memset(&buffer, 0, this->_buffer_max);
	bytesReceveid = recv(client_socket, buffer, this->_buffer_max, 0);
	if (bytesReceveid <= 0)
	{
		close(client_socket);
		FD_CLR(client_socket, &this->_read_master_fd);
		return;
	}
	raw_request = buffer;
	if (this->isIncompleteRequest(client_socket) == true)
	{
		this->_incomplete_requests.find(client_socket)->second.catToBody(raw_request);
		if (this->isIncompleteRequest(client_socket) == false)
		{
			this->_pending_request = this->_incomplete_requests.find(client_socket)->second;
			this->_incomplete_requests.erase(client_socket);
		}
		else
			this->registerReponse(client_socket, "HTTP/1.1 100 CONTINUE");
	}
	else
	{
		Request	request(raw_request);

		if (request.getComplete() == true)
			this->_pending_request = request;
		else
			this->_incomplete_requests.insert(std::pair<int, Request>(client_socket, request));
	}
	FD_SET(client_socket, &this->_write_master_fd);
}

void	TcpListener::handleRequest(int client_socket)
{
	int status_code = 200;
	Route *route = NULL;
	Server *server = getServerByHost(getPortBySocket(&client_socket), _pending_request.getHeader("Host"));

	std::list<Route *> r = server->getRoute();
	for (std::list<Route *>::iterator it = r.begin(); it != r.end(); it++) {
		// std::cout << "our paths : " << (*it)->getPath() << " to compare to " << _pending_request.getPath() << std::endl;
		if (!(*it)->getPath().compare(_pending_request.getPath())) {
			route = *it; // while until every path sent ? like index + img ?
			break;
		}
	}

	// if (route /*&& route->getCgi()*/) {
	// 	std::cout << "YES ROUTE ! method is : " << _pending_request.getMethod() << std::endl;
	// 	if (!route->getExtension().empty()
	// 		|| _pending_request.getMethod() == POST
	// 		|| (_pending_request.getMethod() == GET && _pending_request.getPath().compare("/"))) {
	// 			std::cout << "DEBUT of CGI ??\n";
	// 			Cgi cgi(_pending_request, *route);
	// 			cgi.executeCgi();
	// 			status_code = cgi.getExitCode();
	// 			std::cout << "End of CGI !\n [SATUS CODE =" << status_code << "]\n";
	// 	}
	// }

	Response response(server, status_code, &_pending_request, client_socket);//                create response here
	response.buildResponse(route);
	FD_SET(client_socket, &this->_write_master_fd);
	this->registerReponse(client_socket, response.getResponse());
}


void	TcpListener::registerReponse(int socket, std::string response)
{
	std::map<int, std::string>::iterator	it;

	it = this->_responses.find(socket);
	if (it == this->_responses.end())
		this->_responses.insert(std::pair<int, std::string>(socket, response));
	else
		(*it).second = response;
}

void	TcpListener::writeResponse(int client_socket)
{
	std::cout << "Sending response\n";
	std::string	response = this->getResponse(client_socket);
	send(client_socket, response.c_str(), response.size(), 0);
	FD_CLR(client_socket, &this->_write_master_fd);
}
