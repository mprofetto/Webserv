/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 09:43:00 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/21 14:46:56 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../TcpListener.hpp"

const char	*TcpListener::openFileFailure::what(void) const throw()
{
	return ("File opening failed");
}

const char	*TcpListener::confFileMisconfiguration::what(void) const throw()
{
	return ("Configuration File is Misconfigurated");
}

const char	*TcpListener::confFileMissingDirective::what(void) const throw()
{
	return ("Missing Directive in Configuration File");
}

void	TcpListener::getMaxBodySizeConfig(std::string arg)
{
	int	result;

	for (unsigned int i = 0; i < arg.size(); i++)
	{
		if (isdigit(*(arg.c_str() + i)) == 0)
			throw confFileMisconfiguration();
	}
	if (arg.size() > 7)
		std::cout << "Due to performance limitations, ClientMaxBodySize shouldnt be greater than 2Mo\n. ClientMaxBufferSize set to 2Mo\n";
	result = atoi(arg.c_str());
	if (result > MAXBUFFERSIZE)
		std::cout << "Due to performance limitations, ClientMaxBodySize shouldnt be greater than 2Mo\n. ClientMaxBufferSize set to 2Mo\n";
	else
		this->_buffer_max = result;
}

void	TcpListener::parseConfigurationFile(std::string filename)
{
	std::list<std::string>	token_list = tokenizeConfigurationFile(filename);

	if (token_list.front().compare("clientMaxBodySize") != 0 && token_list.front().compare("server") != 0)
		throw confFileMisconfiguration();
	else if (token_list.front().compare("clientMaxBodySize") == 0)
	{
		token_list.pop_front();
		getMaxBodySizeConfig(token_list.front());
		token_list.pop_front();
	}
	while (token_list.empty() == false)
		token_list = getNextServerConfig(token_list);
	if (this->_servers.empty() == true)
		throw confFileMisconfiguration();
}

std::list<std::string>	TcpListener::getNextDirective(std::list<std::string> token_list, Server	*new_server)
{
	std::string	arg;

	arg = token_list.front();
	if (arg.compare("listen"))
		// token_list = getListenDirective(token_list, new_server);
		;
	else if (arg.compare("server_name"))
		// token_list = getServerNameDirective(token_list, new_server);
		;
	else if (arg.compare("error_page"))
		// token_list = getErrorPagenDirective(token_list, new_server);
		;
	else if (arg.compare("root"))
		// token_list = getRootDirective(token_list, new_server);
		;
	else if (arg.compare("index"))
		// token_list = getIndexDirective(token_list, new_server);
		;
	else if (arg.compare("location"))
		// token_list = getLocationDirective(token_list, new_server);
		;
	else
		throw confFileMisconfiguration();
	(void) new_server;
	return (token_list);
}

std::list<std::string>	TcpListener::getServerDirectives(std::list<std::string> token_list)
{
	Server	*new_server = new Server();
	while (token_list.front().compare("}") != 0)
		token_list = getNextDirective(token_list, new_server);
	if (new_server->getIndex().empty() == true || new_server->getRoot().empty() == true || new_server->getPort() == 0 || new_server->getRoute().empty() == true)
		throw confFileMissingDirective();
	token_list.pop_front();
	return (token_list);
}

std::list<std::string>	TcpListener::getNextServerConfig(std::list<std::string>	token_list)
{
	if (token_list.front().compare("server") != 0)
		throw confFileMisconfiguration();
	token_list.pop_front();
	if (token_list.front().compare("{") != 0)
		throw confFileMisconfiguration();
	token_list.pop_front();
	token_list = getServerDirectives(token_list);
	return (token_list);
}
