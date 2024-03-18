/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:39:42 by mprofett          #+#    #+#             */
/*   Updated: 2024/03/11 15:17:28 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../TcpListener.hpp"

Server			*TcpListener::getServerByHost(int port, std::string host)
{
	std::list<Server *>::const_iterator	it;
	Server 								*temp;
	Server								*result = NULL;

	it = this->_servers.begin();
	if (host.compare("localhost") == 0)
		host = "127.0.0.1";
	while (it != this->_servers.end())
	{
		temp = *it;
		if (result == NULL && temp->getPort() == port)
			result = temp;
		if (temp->getPort() == port && (temp->getHost().compare(host) == 0 || temp->isServerName(host) == true))
			return (temp);
		it++;
	}
	return  (result);
}

int		TcpListener::getPortBySocket(int *socket)
{
	int			port;
	sockaddr_in	addr;
	socklen_t	addr_len = sizeof(sockaddr);

	if (getsockname(*socket, (sockaddr *)&addr, &addr_len) < 0)
		throw socketPortIdentificationFailure();
	port = htons(addr.sin_port);
	return (port);
}

Server	*TcpListener::getServerBySocket(int socket)
{
	std::list<Server *>::const_iterator	it;
	Server 								*result;

	it = this->_servers.begin();
	while (it != this->_servers.end())
	{
		result = *it;
		if (result->getSocket() == socket)
			return (result);
		it++;
	}

	return (NULL);
}

void	TcpListener::printServers(void) const
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

std::string		TcpListener::getResponse(int socket)
{
	std::map<int, std::string>::iterator	it;

	it = this->_responses.find(socket);
	if (it == this->_responses.end())
		return ("");
	else
		return ((*it).second);
}

bool			TcpListener::isIncompleteRequest(int socket)
{
	std::map<int, Request>::iterator	it;

	it = this->_incomplete_requests.find(socket);
	if (it == this->_incomplete_requests.end())
		return (false);
	return (true);
}

