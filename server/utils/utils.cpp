/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:39:42 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/26 11:30:36 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../TcpListener.hpp"

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
	result = NULL;
	return  (NULL);
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
