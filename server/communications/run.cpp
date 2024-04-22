/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/04/22 13:40:45 by achansar         ###   ########.fr       */
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

void	TcpListener::handleRequest(int client_socket)
{

	// std::cout << "Right before habnleRequest, uri is : " << _pending_request.getPath() << std::endl;

	int status_code = 200;
	Route *route = NULL;
	Server *server = getServerByHost(getPortBySocket(&client_socket), _pending_request.getHeader("Host"));

	std::string checkRoute = _pending_request.getPath();
	std::string cgi_ext = checkRoute;
    size_t dotPos = cgi_ext.find_last_of('.');
    if (dotPos != std::string::npos) {
        cgi_ext = cgi_ext.c_str() + dotPos;
	}
	if (!checkRoute.empty() && checkRoute[checkRoute.size() - 1] != '/')
		checkRoute += "/";
	std::list<Route *> r = server->getRoute();
	for (std::list<Route *>::iterator it = r.begin(); it != r.end(); it++) {
		std::cout << "CHECKING THE ROUTES !:\ncheckRoute : " << checkRoute << " | Path : " << (*it)->getPath() << std::endl;
		std::cout << "Root : " << (*it)->getRoot() << " | Path : " << (*it)->getPath() << " | Extension : " << (*it)->getExtension() << std::endl;
		if (!(*it)->getPath().compare(checkRoute)) { // la deuxieme condition necessaire car ne rentre pas dans cgi sinon
			route = *it;
			break;
		}
		if ((*it)->getPath() == "/usr/bin/python3") {
			if (cgi_ext == ".py"){
				route = *it;
				break;
			}
		}
		if ((*it)->getPath() == "/usr/bin/perl") {
			if (cgi_ext == ".pl"){
				route = *it;
				break;
			}
		}
	}

	
	// for (std::list<Route *>::iterator it = r.begin(); it != r.end(); it++) {
	// 	std::string path = "." + (*it)->getPath() + _pending_request.getPath();
	// 	if (path == ".//index.html")
	// 		path = "./index.html";
	// 	std::cout << "IN NEW CHECK< PATH IS " << path << std::endl;
	// 	if (isFile(path.c_str())) {
	// 		route = *it;
	// 		break;
	// 	} 
	// }
	

	// std::cout << "Right after CGI, uri is : " << _pending_request.getPath() << std::endl;

	Response* response = new Response(server, status_code, &_pending_request, client_socket);//                create response here
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
		std::cerr << "Error on sending response." << std::endl;
	else
		response->addToBytesSend(bytesSent);
	if (response->getBytesSend() == response->getResponse().size() || bytesSent <= 0)
	{
		FD_CLR(client_socket, &this->_write_master_fd);
		delete response;
		this->_responses.erase(client_socket);
	}
}
