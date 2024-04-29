/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/04/29 17:35:35 by achansar         ###   ########.fr       */
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
	int			client_socket;
	sockaddr	client_addr;
	socklen_t	addr_size = sizeof(sockaddr);

	client_socket = accept(server->getSocket(), &client_addr, &addr_size);
	if (client_socket < 0)
		throw socketAcceptingNewConnectionFailure();
	FD_SET(client_socket, &this->_read_master_fd);
}

bool isFile(const char* path) {
	struct stat fileInfo;
	if (stat(path, & fileInfo) != 0) {
		return false;
	}
	return S_ISREG(fileInfo.st_mode);
}

Route*	TcpListener::selectRoute(Server *server, std::string location, std::string cgi_ext) {

	Route *route = NULL;
	std::list<Route *> r = server->getRoute();

	for (std::list<Route *>::iterator it = r.begin(); it != r.end(); it++) {
		std::string path = "." + (*it)->getPath() + location;
		if (location == (*it)->getPath()) {
			route = *it;
			break;
		}
		if ((isFile(path.c_str()) && cgi_ext != ".py" && cgi_ext != ".pl")
			|| ((*it)->getPath() == "/usr/bin/python3" && cgi_ext == ".py")
			|| ((*it)->getPath() == "/usr/bin/perl" && cgi_ext == ".pl")) {
			route = *it;
			break;
		}
	}
	return route;
}

std::string	TcpListener::buildURI(std::string uri, Server *server, std::string cgi_ext, Route **route) {

	size_t pos = uri.find("/");
	pos = uri.find("/", pos + 1);

	if (pos == std::string::npos) {
		if (uri.find(".") != std::string::npos)
			pos = 1;
		else
			pos = uri.length();
	}
	std::string location = uri.substr(0, pos);
	std::string resource = uri.substr(pos);
	
	std::cout << "Location is " << location << std::endl;
	std::cout << "Resource is " << resource << std::endl;

	*route = selectRoute(server, location, cgi_ext);
	if (*route) {
		std::cout << "YES ROUTE ! URI : " << uri << " and path : " << (*route)->getPath() << std::endl;
		if (((*route)->getPath() == uri))
			uri = (*route)->getRoot();
		else
			uri = (*route)->getRoot() + resource;
	}
	else
		uri = "/";
	return uri;
}

void	TcpListener::handleRequest(int client_socket)
{
	int status_code = 200;
	Route *route = NULL;
	Server *server = getServerByHost(getPortBySocket(&client_socket), _pending_request.getHeader("Host"));

	std::string cgi_ext = _pending_request.getPath();
	
    size_t dotPos = cgi_ext.find_last_of('.');
    if (dotPos != std::string::npos) {
        cgi_ext = cgi_ext.c_str() + dotPos;
	}

	// std::string uri = _pending_request.getPath();

	std::string uri = buildURI(_pending_request.getPath(), server, cgi_ext, &route);


//======================================================== to put in fucntion
	// size_t pos = uri.find("/");
	// pos = uri.find("/", pos + 1);

	// if (pos == std::string::npos) {
	// 	if (uri.find(".") != std::string::npos)
	// 		pos = 1;
	// 	else
	// 		pos = uri.length();
	// }
	// std::string location = uri.substr(0, pos);
	// std::string resource = uri.substr(pos);
	
	
	// std::cout << "Location is " << location << std::endl;
	// std::cout << "Resource is " << resource << std::endl;

	// route = selectRoute(server, location, cgi_ext);
	// if (route) {
	// 	// std::cout << "URI : " << uri << " and path : " << route->getPath() << std::endl;
	// 	if ((route->getPath() == uri))
	// 		uri = route->getRoot();
	// 	else
	// 		uri = route->getRoot() + resource;
	// }
	// else
	// 	uri = "/";
	if (route) {
		std::cout << "YES ROUTE 2 !!" << std::endl;
	}
	std::cout << "New URI is " << uri << "\n" << std::endl;
//==============================================================
	 
	Response* response = new Response(server, status_code, &_pending_request, client_socket, uri);
	response->buildResponse(route);
	FD_SET(client_socket, &this->_write_master_fd);
	this->registerResponse(client_socket, response);
}

void	TcpListener::registerResponse(int socket, Response *response)
{
	std::map<int, Response*>::iterator	it;

	it = this->_responses.find(socket);
	if (it == this->_responses.end())
		this->_responses.insert(std::pair<int, Response*>(socket, response));
	else
		it->second = response;
}

void	TcpListener::writeResponse(int client_socket)
{
	Response	*response = this->_responses.find(client_socket)->second;
	std::string	to_send = response->getNextChunk();
	ssize_t		bytesSent = send(client_socket, to_send.c_str(), to_send.size(), 0);

	if (bytesSent == -1)
	{
		std::cerr << "Error on sending response." << std::endl;
		FD_CLR(client_socket, &this->_write_master_fd);
		close(client_socket);
	}
	else
		response->addToBytesSend(bytesSent);
	if (response->getBytesSend() == response->getResponse().size() || bytesSent == 0)
	{
		FD_CLR(client_socket, &this->_write_master_fd);
		close(client_socket);
		delete response;
		this->_responses.erase(client_socket);
	}
}
