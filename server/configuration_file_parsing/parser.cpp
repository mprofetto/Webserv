/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 09:43:00 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/30 13:37:28 by mprofett         ###   ########.fr       */
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

void	TcpListener::parseConfigurationFile(std::string filename)
{
	std::list<std::string>	token_list = tokenizeConfigurationFile(filename);

	if (token_list.empty() == true)
		throw confFileMisconfiguration();
	if (token_list.front().compare("clientMaxBodySize") != 0 && token_list.front().compare("server") != 0)
		throw confFileMisconfiguration();
	else if (token_list.front().compare("clientMaxBodySize") == 0)
		token_list = getMaxBodySizeDirective(token_list);
	while (token_list.empty() == false)
		token_list = getNextServerConfig(token_list);
}

std::list<std::string>	TcpListener::getNextServerDirective(std::list<std::string> token_list, Server	*new_server)
{
	std::string	arg;

	arg = token_list.front();
	if (arg.compare("listen") == 0)
		token_list = getListenDirective(token_list, new_server);
	else if (arg.compare("host") == 0)
		token_list = getHostDirective(token_list, new_server);
	else if (arg.compare("server_name") == 0)
		token_list = getServerNameDirective(token_list, new_server);
	else if (arg.compare("error_page") == 0)
		token_list = getErrorPageDirective(token_list, new_server);
	else if (arg.compare("root") == 0)
		token_list = getRootDirective(token_list, new_server);
	else if (arg.compare("index") == 0)
		token_list = getIndexDirective(token_list, new_server);
	else if (arg.compare("location") == 0)
		token_list = getLocationDirective(token_list, new_server);
	else
		throw confFileMisconfiguration();
	return (token_list);
}

std::list<std::string>	TcpListener::getServerDirectives(std::list<std::string> token_list)
{
	Server	*new_server = new Server();
	while (token_list.front().compare("}") != 0)
		token_list = getNextServerDirective(token_list, new_server);
	if (new_server->getIndex().empty() == true)
		new_server->addIndex("index.html");
	token_list.pop_front();
	if (this->serverAlreadyExistForThisPortAndHost(new_server) == false)
		this->_servers.push_back(new_server);
	else
	{
		std::cerr << "Server already setup for port " << new_server->getPort() << " and host " << new_server->getHost() << std::endl;
		delete new_server;
	}
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
