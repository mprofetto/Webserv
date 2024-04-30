/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 09:45:12 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/30 11:49:12 by mprofett         ###   ########.fr       */
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

const char	*Server::invalidSocket::what(void) const throw()
{
	return ("Socket given is not valid or server has already open too many sockets");
}

std::map<std::string, std::string>	Server::buildMimeTypes() {

	std::map<std::string, std::string> MIMEtypes;
    MIMEtypes.insert(std::make_pair(".html", "text/html"));
    MIMEtypes.insert(std::make_pair(".css", "text/css"));
    MIMEtypes.insert(std::make_pair(".txt", "text/plain"));
    MIMEtypes.insert(std::make_pair(".jpg", "image/jpeg"));
    MIMEtypes.insert(std::make_pair(".jpeg", "image/jpeg"));
    MIMEtypes.insert(std::make_pair(".png", "image/png"));
    MIMEtypes.insert(std::make_pair(".ico", "image/x-icon"));
    MIMEtypes.insert(std::make_pair(".pdf", "application/pdf"));
    MIMEtypes.insert(std::make_pair("default", "text/html"));
	return MIMEtypes;
}

Server::Server() : _host(LOCALHOST), _root(""), _port(80), _socket(-1)
{
	this->_index.push_back("index.html");
	this->_mime_types = buildMimeTypes();
	return;
}

Server::~Server()
{
	clearList(this->_routes);
	return;
}

std::string					Server::getMimeType(std::string ext) const
{
	std::map<std::string, std::string>::const_iterator it = _mime_types.find(ext);
	if (it != _mime_types.end()) {
		return it->second;
	} else {
		return "application/octet-stream";
	}
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

std::string					Server::getHost(void) const
{
	return (this->_host);
}

int							Server::getPort(void) const
{
	return (this->_port);
}

std::string					Server::getRoot(void) const
{
	return (this->_root);
}

int							Server::getSocket(void) const
{
	return (this->_socket);
}

std::map<int, std::string>	Server::getErrorPages(void) const
{
	return (this->_error_pages);
}

std::string					Server::getDefaultErrorPage(int error_code) const
{
	unsigned long		pos;
	std::stringstream	mystream;
	std::string			my_error_code;
	std::string			result("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Error Page</title></head><body><h1>Error $</h1></body></html>");

	mystream << error_code;
	my_error_code = mystream.str();
	pos = result.find_first_of("$");
	result.replace(pos, 1, my_error_code);
	return (result);
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

void						Server::setHost(std::string host)
{
	if (host.compare("localhost") == 0)
		host = "127.0.0.1";
	this->_host = host;
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

void						Server::setSocket(int socket)
{
	if (socket > 0 && socket < FD_SETSIZE)
		this->_socket = socket;
	else
		throw invalidSocket();

}

bool						Server::isServerName(std::string name) const
{
	std::list<std::string>::const_iterator		it_server_names;

	if (this->_server_names.empty() == false)
		{
			it_server_names = this->_server_names.begin();
			while (it_server_names != this->_server_names.end())
			{
				if (name.compare(" " + *it_server_names + "\r") == 0)
				{
					std::cout << "true\n";
					return (true);
				}
				it_server_names++;
			}
		}
	return (false);
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
	std::cout << "Host: " << this->_host << "\n";
	(void)route;
	// if (this->_routes.empty() == false)
	// {
	// 	std::cout << "Nbr of Routes is " << this->_routes.size() << "\n";
	// 	it_routes = this->_routes.begin();
	// 	while (it_routes != this->_routes.end())
	// 	{
	// 		route = *it_routes;
	// 		std::cout << "********************\n";
	// 		route->printRoute();
	// 		std::cout << "********************\n";
	// 		it_routes++;
	// 	}
	// }
	// else
	// 	std::cout << "No routes setup for this server\n";
	// if (this->_server_names.empty() == false)
	// {
	// 	it_server_names = this->_server_names.begin();
	// 	std::cout << "Server Names: ";
	// 	while (it_server_names != this->_server_names.end())
	// 	{
	// 		str = *it_server_names;
	// 		std::cout << " " << str;
	// 		it_server_names++;
	// 	}
	// 	std::cout << "\n";
	// }
	// if (this->_index.empty() == false)
	// {
	// 	it_index = this->_index.begin();
	// 	std::cout << "Index: ";
	// 	while (it_index != this->_index.end())
	// 	{
	// 		str = *it_index;
	// 		std::cout << " " << str;
	// 		it_index++;
	// 	}
	// 	std::cout << "\n";
	// }
	// if (this->_error_pages.empty() == false)
	// {
	// 	it_error_pages = this->_error_pages.begin();
	// 	while (it_error_pages != this->_error_pages.end())
	// 	{
	// 		error_page = *it_error_pages;
	// 		std::cout << "Error Pages: " << error_page.first << " " << error_page.second << "\n";
	// 		it_error_pages++;
	// 	}
	// }
}
