/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directives.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:25:19 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/23 14:05:14 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../TcpListener.hpp"

std::list<std::string>	TcpListener::getMaxBodySizeDirective(std::list<std::string>	token_list)
{
	int	result;
	std::string	bodySize;

	token_list = popFrontToken(token_list);
	bodySize = token_list.front();
	isDigit(bodySize);
	if (bodySize.size() > 7)
		std::cout << "Due to performance limitations, ClientMaxBodySize shouldnt be greater than 2Mo\n. ClientMaxBufferSize set to 2Mo\n";
	result = atoi(bodySize.c_str());
	if (result > MAXBUFFERSIZE)
		std::cout << "Due to performance limitations, ClientMaxBodySize shouldnt be greater than 2Mo\n. ClientMaxBufferSize set to 2Mo\n";
	else
		this->_buffer_max = result;
	token_list = popFrontToken(token_list);
	return (token_list);
}

std::list<std::string>	TcpListener::getListenDirective(std::list<std::string> token_list, Server *new_server)
{
	std::string	port;
	int			result;

	token_list = popFrontToken(token_list);
	port = token_list.front();
	isDigit(port);
	if (port.size() > 6)
		throw confFileMisconfiguration();
	result = atoi(port.c_str());
	new_server->setPort(result);
	token_list = popFrontToken(token_list);
	if (token_list.front().compare(";") != 0)
		throw confFileMisconfiguration();
	token_list = popFrontToken(token_list);
	return (token_list);
}

std::list<std::string>	TcpListener::getHostDirective(std::list<std::string> token_list, Server *new_server)
{
	std::string	host;

	token_list = popFrontToken(token_list);
	host = token_list.front();
	new_server->setipAddress(host);
	token_list = popFrontToken(token_list);
	if (token_list.front().compare(";") != 0)
		throw confFileMisconfiguration();
	token_list = popFrontToken(token_list);
	return (token_list);
}

std::list<std::string>	TcpListener::getRootDirective(std::list<std::string> token_list, Server *new_server)
{
	std::string	root;

	token_list = popFrontToken(token_list);
	root = token_list.front();
	new_server->setRoot(root);
	token_list = popFrontToken(token_list);
	if (token_list.front().compare(";") != 0)
		throw confFileMisconfiguration();
	token_list = popFrontToken(token_list);
	return (token_list);
}

std::list<std::string>	TcpListener::getIndexDirective(std::list<std::string> token_list, Server *new_server)
{
	std::string	name;

	token_list = popFrontToken(token_list);
	name = token_list.front();
	while(name.compare(";") != 0)
	{
		new_server->addIndex(name);
		token_list = popFrontToken(token_list);
		name = token_list.front();
	}
	token_list = popFrontToken(token_list);
	return (token_list);
}

std::list<std::string>	TcpListener::getServerNameDirective(std::list<std::string> token_list, Server *new_server)
{
	std::string	name;

	token_list = popFrontToken(token_list);
	name = token_list.front();
	while(name.compare(";") != 0)
	{
		new_server->addServerName(name);
		token_list = popFrontToken(token_list);
		name = token_list.front();
	}
	token_list = popFrontToken(token_list);
	return (token_list);
}

std::list<std::string>	TcpListener::getErrorPageDirective(std::list<std::string> token_list, Server *new_server)
{
	int			code;
	std::string	code_str;
	std::string	page;

	token_list = popFrontToken(token_list);
	code_str = token_list.front();
	for (unsigned int i = 0; i < code_str.size(); i++)
	{
		if (isdigit(*(code_str.c_str() + i)) == 0)
			throw confFileMisconfiguration();
	}
	if (code_str.size() > 3)
		throw confFileMisconfiguration();
	code = atoi(code_str.c_str());
	token_list = popFrontToken(token_list);
	page = token_list.front();
	new_server->addErrorPage(code, page);
	token_list = popFrontToken(token_list);
	if (token_list.front().compare(";") != 0)
		throw confFileMisconfiguration();
	token_list = popFrontToken(token_list);
	return (token_list);
}

std::list<std::string>	TcpListener::getLocationDirective(std::list<std::string> token_list, Server *new_server)
{
	Route		*new_route = new Route();
	std::string	arg;

	token_list = popFrontToken(token_list);
	arg = token_list.front();
	if (*(arg.c_str()) == '.')
		new_route->setExtension(arg);
	else if (*(arg.c_str()) == '/')
		new_route->setPath(arg);
	else
		throw confFileMisconfiguration();
	token_list = popFrontToken(token_list);
	arg = token_list.front();
	if (arg.compare("{") != 0)
		throw confFileMisconfiguration();
	token_list = popFrontToken(token_list);
	while (token_list.front().compare("}") != 0)
		token_list = getNextLocationDirective(token_list, new_route);
	token_list = popFrontToken(token_list);
	new_server->addRoute(new_route);
	return (token_list);
}

std::list<std::string>	TcpListener::getNextLocationDirective(std::list<std::string> token_list, Route *new_route)
{
	std::string	arg;

	arg = token_list.front();
	if (arg.compare("autoindex") == 0)
		token_list = getAutoIndexDirective(token_list, new_route);
	else if (arg.compare("allow_methods") == 0)
		token_list = getAllowMethodsDirective(token_list, new_route);
	else if (arg.compare("proxy_pass") == 0)
		token_list = getProxyPassDirective(token_list, new_route);
	else if (arg.compare("cgi_path") == 0)
		token_list = getCgiPathDirective(token_list, new_route);
	else if (arg.compare("root") == 0)
		token_list = getRootDirective(token_list, new_route);
	else if (arg.compare("index") == 0)
		token_list = getIndexDirective(token_list, new_route);
	else
		throw confFileMisconfiguration();
	return (token_list);
}

std::list<std::string>	TcpListener::getIndexDirective(std::list<std::string> token_list, Route *new_route)
{
	std::string	name;

	token_list = popFrontToken(token_list);
	name = token_list.front();
	while(name.compare(";") != 0)
	{
		new_route->addIndex(name);
		token_list = popFrontToken(token_list);
		name = token_list.front();
	}
	token_list = popFrontToken(token_list);
	return (token_list);
}

std::list<std::string>	TcpListener::getRootDirective(std::list<std::string> token_list, Route *new_route)
{
	std::string	root;

	token_list = popFrontToken(token_list);
	root = token_list.front();
	new_route->setRoot(root);
	token_list = popFrontToken(token_list);
	if (token_list.front().compare(";") != 0)
		throw confFileMisconfiguration();
	token_list = popFrontToken(token_list);
	return (token_list);
}

std::list<std::string>	TcpListener::getAutoIndexDirective(std::list<std::string> token_list, Route *new_route)
{
	std::string	arg;

	token_list = popFrontToken(token_list);
	arg = token_list.front();
	if (arg.compare("off") == 0)
		new_route->setAutoindex(false);
	else if (arg.compare("on") == 0)
		new_route->setAutoindex(true);
	else
		throw confFileMisconfiguration();
	token_list = popFrontToken(token_list);
	if (token_list.front().compare(";") != 0)
		throw confFileMisconfiguration();
	token_list = popFrontToken(token_list);
	return (token_list);
}

std::list<std::string>	TcpListener::getProxyPassDirective(std::list<std::string> token_list, Route *new_route)
{
	std::string	arg;

	token_list = popFrontToken(token_list);
	new_route->setRedirection(token_list.front());
	token_list = popFrontToken(token_list);
	if (token_list.front().compare(";") != 0)
		throw confFileMisconfiguration();
	token_list = popFrontToken(token_list);
	return (token_list);
}

std::list<std::string>	TcpListener::getCgiPathDirective(std::list<std::string> token_list, Route *new_route)
{
	std::string	arg;

	token_list = popFrontToken(token_list);
	new_route->setPath(token_list.front());
	token_list = popFrontToken(token_list);
	if (token_list.front().compare(";") != 0)
		throw confFileMisconfiguration();
	token_list = popFrontToken(token_list);
	new_route->setCgi(true);
	return (token_list);
}

std::list<std::string>	TcpListener::getAllowMethodsDirective(std::list<std::string> token_list, Route *new_route)
{
	std::string	name;

	token_list = popFrontToken(token_list);
	name = token_list.front();
	while(name.compare(";") != 0)
	{
		if (name.compare("GET") == 0)
			new_route->setGet(true);
		else if (name.compare("POST") == 0)
			new_route->setPost(true);
		else if (name.compare("DELETE") == 0)
			new_route->setDelete(true);
		else
			throw confFileMisconfiguration();
		token_list = popFrontToken(token_list);
		name = token_list.front();
	}
	token_list = popFrontToken(token_list);
	return (token_list);
}
