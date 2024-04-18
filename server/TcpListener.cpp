/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpListener.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 09:37:47 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/17 10:05:30 by mprofett         ###   ########.fr       */
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

TcpListener::TcpListener(std::string configfile) : _buffer_max(MAXBUFFERSIZE), _socket_nbr(0)
{
	parseConfigurationFile(configfile);
	return;
}

void		TcpListener::initTcpListener()
{
	std::list<Server *>::const_iterator	it;
	Server								*server;

	FD_ZERO(&this->_read_master_fd);
	FD_ZERO(&this->_write_master_fd);
	it = this->_servers.begin();
	while (it != this->_servers.end())
	{
		server = *it;
		initServer(server);
		it++;
	}
}

void		TcpListener::initServer(Server *server)
{
	int	option_value = 1;

	server->setSocket(socket(AF_INET, SOCK_STREAM, 0));
	if (fcntl(server->getSocket(), F_SETFL, O_NONBLOCK) < 0)
		throw socketInitializationFailure();
	if (setsockopt(server->getSocket(), SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value)) < 0)
		throw socketConfigurationFailure();
	if (setsockopt(server->getSocket(), SOL_SOCKET, SO_REUSEPORT, &option_value, sizeof(option_value)) < 0)
		throw socketConfigurationFailure();
	bindSocket(server);
	if (listen(server->getSocket(), SOMAXCONN) < 0)
		throw socketListeningFailure();
	FD_SET(server->getSocket(), &this->_read_master_fd);
}

void	TcpListener::bindSocket(Server *server)
{
	sockaddr_in server_socket_addr;

	server_socket_addr.sin_family = AF_INET;
	server_socket_addr.sin_port = htons(server->getPort());
	server_socket_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(server_socket_addr.sin_zero), 0, 8);
	if (bind(server->getSocket(), (sockaddr *)&server_socket_addr, sizeof(sockaddr)) < 0)
		throw socketBindingFailure();
}

std::string	generateSessionId(void)
{
	std::stringstream ss;

	srand(time(NULL) + rand());
	for(int i = 0; i < 64; i++)
	{
		int j = rand() % 127;
		while(j < 32)
			j = rand() % 127;
		ss << char(j);
	}
	return (ss.str());
}

std::string	generateExpirationDate(time_t duration)
{
	time_t				ltime;

	time(&ltime);
	ltime += duration;

	std::stringstream	ss(asctime(gmtime(&ltime)));

	std::string	weekDay;
	std::string	day;
	std::string	month;
	std::string	year;
	std::string	hours;

	std::getline(ss, weekDay, ' ');
	std::getline(ss, month, ' ');
	std::getline(ss, day, ' ');
	std::getline(ss, hours, ' ');
	std::getline(ss, year, ' ');

	std::string	result = weekDay + ", " + day + " "+ month + " " + year + hours + " GMT";
	result.replace(result.find("\n"), 1, " ");
	return (result);
}
