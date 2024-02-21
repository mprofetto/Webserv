/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 09:45:12 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/21 14:44:23 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

const char	*Server::invalidPortRange::what(void) const throw()
{
	return ("Port not in 1-65353 range");
}

Server::Server() : _index(""), _ipAdress(LOCALHOST), _root(""), _port(0)
{
	return;
}

Server::~Server()
{
	return;
}

std::list<Route>			Server::getRoute(void) const
{
	return (this->_routes);
}

std::list<std::string>		Server::getServerNames(void) const
{
	return (this->_server_names);
}

std::string					Server::getIndex(void) const
{
	return (this->_index);
}

std::string					Server::getIpAdress(void) const
{
	return (this->_ipAdress);
}

int							Server::getPort(void) const
{
	return (this->_port);
}

std::string					Server::getRoot(void) const
{
	return (this->_root);
}

std::map<int, std::string>	Server::getErrorPages(void) const
{
	return (this->_error_pages);
}

void						Server::addRoute(Route route)
{
	this->_routes.push_back(route);
}

void						Server::addServerName(std::string name)
{
	this->_server_names.push_back(name);
}

void						Server::addErrorPage(int code, std::string path)
{
	std::pair<int, std::string>	new_elem(code, path);

	this->_error_pages.insert(new_elem);
}

void						Server::setIndex(std::string filename)
{
	this->_index = filename;
}

void						Server::setIpAdress(std::string ip)
{
	this->_ipAdress = ip;
}

void						Server::setPort(int port)
{
	if (port >= 1 && port <= 65535)
		this->_port = port;
	else
		throw invalidPortRange();
}
void						Server::setRoot(std::string path)
{
	this->_root = path;
}
