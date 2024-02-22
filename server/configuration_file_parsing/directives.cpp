/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directives.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:25:19 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/22 14:10:48 by mprofett         ###   ########.fr       */
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
	new_server->setIpAdress(host);
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

