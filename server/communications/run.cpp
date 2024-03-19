/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:56:28 by mprofett          #+#    #+#             */
/*   Updated: 2024/03/19 15:15:33 by nesdebie         ###   ########.fr       */
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

void	getFullPath(Route *route, Response& response) {

	if (response.getStatusCode() == 200) {
		// may use std::filesystem::is_directory() when segfault is fixed
		std::string root = route->getRoot() + "/";
		std::string index = route->getIndex().front();//         only gets first index. it's temporary
		std::string p = root + index;

	// TO DO : CHECK IF PATH IS VALID
	// If not,
	// p = status-code correspondant
	// sinon,
		response.setPath(p);
	}
	return;
}

int CGIfucntion() {
	return 200;
}

void	TcpListener::handleRequest(int client_socket)
{
	int status_code = 200;
	Route *route = NULL;
	Server *server = getServerByHost(getPortBySocket(&client_socket), _pending_request.getHeader("Host"));
	int i = 1;

	std::list<Route *> r = server->getRoute();

	std::cout << "List of routes is size : " << r.size() << std::endl;
	for (std::list<Route *>::iterator it = r.begin(); it != r.end(); it++) {
		i++;
		// if (it == r.end()) { .  -> visiblement ne fonction pas, 4TH	 root is empty ?
		if (i >= 4) {
			status_code = 404;
		}
		if (!(*it)->getPath().compare(_pending_request.getPath())) {
			route = *it; // while until every path sent ? like index + img ?
			break;
		}
	}

	/////////////////////// TO CGI ?

	// std::cout << "EXTENSION : " << route->getExtension() << std::endl;

	if (!route->getExtension().empty()
		|| _pending_request.getRequestLine().getMethod() == POST
		|| (_pending_request.getRequestLine().getMethod() == GET && _pending_request.getPath().compare("/"))) {
	// 		/////////////////
		std::cout << "Yes ??" << std::endl;
	// 		status_code = CGIfucntion();// temporary function for my tests
	// 		// il faut donc renvoyer un int pour status_code

	// 		// Ici une fonction vers la partie/classe CGI

	// 		////////////////////
	}
	////////////////////////
	if (route) {
		Cgi cgi(_pending_request, *route);
		cgi.executeCgi();
		status_code = cgi.getExitCode();
		Response response(server, status_code, _pending_request.getRequestLine().getMethod());//                create response here
		getFullPath(route, response);

		response.buildResponse(_pending_request);
		FD_SET(client_socket, &this->_write_master_fd);
		this->registerReponse(client_socket, response.getResponse());
	}
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
