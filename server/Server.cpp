/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 09:45:12 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/23 14:21:29 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

const char	*Server::invalidPortRange::what(void) const throw()
{
	return ("Port not in 1-65353 range");
}

const char	*Server::invalidErrorPageCode::what(void) const throw()
{
	return ("Html Error Code not in use for this webserver");
}

const char	*Server::invalidErrorPagePath::what(void) const throw()
{
	return ("Error page file doesnt exists or require higher permission");
}

const char	*Server::invalidHostFormat::what(void) const throw()
{
	return ("Host not formatted as XXX.XXX.XXX.XXX or not in a range from 0.0.0.0. to 255.255.255.255");
}

Server::Server() : _ipAddress(LOCALHOST), _root(""), _port(80)
{
	return;
}

Server::~Server()
{
	clearList(this->_routes);
	return;
}

std::list<Route *>			Server::getRoute(void) const
{
	return (this->_routes);
}

std::list<std::string>		Server::getServerNames(void) const
{
	return (this->_server_names);
}

std::list<std::string>		Server::getIndex(void) const
{
	return (this->_index);
}

std::string					Server::getipAddress(void) const
{
	return (this->_ipAddress);
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

void						Server::addRoute(Route *route)
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

	if (!(code > 300 && code < 310) && !(code > 400 && code < 527))
		throw invalidErrorPageCode();
	else if (access(path.c_str(), R_OK) != 0)
		throw invalidErrorPagePath();
	this->_error_pages.insert(new_elem);
}

void						Server::addIndex(std::string filename)
{
	this->_index.push_back(filename);
}

std::string	Server::convertIpAddress(std::vector<std::string> address)
{
	std::string			str;
	std::stringstream	str_stream;
	std::string			result;
	int			nbr;

	if (address.size() != 4)
		throw invalidHostFormat();
	for (unsigned int i = 0; i < address.size(); i++)
	{
		str = address[i];
		if (str.size() > 3)
			throw invalidHostFormat();

		for (unsigned int j = 0; j < str.size(); j++)
		{
			if (isdigit(*(str.c_str() + j)) == 0)
				throw invalidHostFormat();

		}
		nbr = atoi(str.c_str());
		if (nbr > 255)
			throw invalidHostFormat();
		str_stream << nbr;
		if (i < 3)
			str_stream << ".";
	}
	result = str_stream.str();
	return (result);
}

void						Server::setipAddress(std::string ip)
{
	std::vector<std::string>	address;
	std::stringstream			str_stream(ip);
	std::string					str;

	if (ip.compare("localhost"))
		this->_ipAddress = "127.0.0.1";
	else
	{
		while (getline(str_stream, str, '.'))
			address.push_back(str);
		this->_ipAddress = convertIpAddress(address);
	}
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

void						Server::printDatas(void) const
{
	std::list<Route *>::const_iterator			it_routes;
	std::list<std::string>::const_iterator		it_server_names;
	std::list<std::string>::const_iterator		it_index;
	std::map<int, std::string>::const_iterator	it_error_pages;
	std::string									str;
	Route										*route;
	std::pair<int, std::string>					error_page;

	std::cout << "Port: " << this->_port << "\n";
	std::cout << "Root: " << this->_root << "\n";
	std::cout << "Ip Adress: " << this->_ipAddress << "\n";
	if (this->_routes.empty() == false)
	{
		std::cout << "Nbr of Routes is " << this->_routes.size() << "\n";
		it_routes = this->_routes.begin();
		while (it_routes != this->_routes.end())
		{
			route = *it_routes;
			std::cout << "********************\n";
			route->printRoute();
			std::cout << "********************\n";
			it_routes++;
		}
	}
	else
		std::cout << "No routes setup for this server\n";
	if (this->_server_names.empty() == false)
	{
		it_server_names = this->_server_names.begin();
		std::cout << "Server Names: ";
		while (it_server_names != this->_server_names.end())
		{
			str = *it_server_names;
			std::cout << " " << str;
			it_server_names++;
		}
		std::cout << "\n";
	}
	if (this->_index.empty() == false)
	{
		it_index = this->_index.begin();
		std::cout << "Index: ";
		while (it_index != this->_index.end())
		{
			str = *it_index;
			std::cout << " " << str;
			it_index++;
		}
		std::cout << "\n";
	}
	if (this->_error_pages.empty() == false)
	{
		it_error_pages = this->_error_pages.begin();
		while (it_error_pages != this->_error_pages.end())
		{
			error_page = *it_error_pages;
			std::cout << "Error Pages: " << error_page.first << " " << error_page.second << "\n";
			it_error_pages++;
		}
	}
}
