/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpListener.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 09:32:35 by mprofett          #+#    #+#             */
/*   Updated: 2024/03/28 15:07:16 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPLISTENER_HPP
# define TCPLISTENER_HPP

# include <fstream>
# include <iostream>
# include <list>
# include <map>
# include <string>
# include <sstream>
# include <fcntl.h>
# include <exception>
# include <errno.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <arpa/inet.h>
# include <unistd.h>
# include "Server.hpp"
# include "Route.hpp"
# include "../request_manager/includes/Request.hpp"
# include "../utils.hpp"
# include "../response_manager/Response.hpp"


# define MAXBUFFERSIZE 2097152

class Server;
class Response;
class Request;

class TcpListener
{
	public:

		class	socketInitializationFailure : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	socketConfigurationFailure : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	socketBindingFailure : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	socketListeningFailure : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	socketSelectFailure : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	socketAcceptingNewConnectionFailure : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	socketPortIdentificationFailure : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	openFileFailure : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	confFileMisconfiguration : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	confFileMissingDirective : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		TcpListener(std::string	configfile);
		~TcpListener();

		void			initTcpListener();
		void			runTcpListener();

		//temporary debug methods
		void			printServers() const;

	private:

		long long int						_buffer_max;
		int									_socket_nbr;
		int									_socket;
		fd_set								_read_master_fd;
		fd_set								_write_master_fd;
		std::list<Server *>					_servers;
		Request								_pending_request;
		// std::map<int, Request>		_incomplete_requests;
		std::map<int, std::string>			_incomplete_requests;
		std::map<int, std::string>			_responses;

		//Init Methods
		void					bindSocket(Server *server);
		void					initServer(Server *server);

		//Communication Methods
		void					handleNewConnection(Server *server);
		void					readRequest(int socket);
		void					create_new_incoming_request(int client_socket, int size, char *buffer);
		void					update_incoming_request(int client_socket, int size, char *buffer);
		void					register_new_pending_request(int client_socket, char *buffer);
		bool					isIncompleteRequest(int socket);
		void					registerReponse(int socket, std::string response);
		void					writeResponse(int socket);
		void					handleRequest(int client_socket); /*this function store response with this->registerResponse(std::string response, int socket);*/

		//Utils
		Server					*getServerByHost(int port, std::string host);
		Server					*getServerBySocket(int socket);//return NULL if there is no server for this socket
		int						getPortBySocket(int *socket);
		std::string				getResponse(int socket);

		//Parse config file
		void					isDigit(std::string) const;
		std::list<std::string>	popFrontToken(std::list<std::string> token_list);
		std::list<std::string>	getNextLocationDirective(std::list<std::string> token_list, Route *new_route);
		std::list<std::string>	getAutoIndexDirective(std::list<std::string> token_list, Route *new_route);
		std::list<std::string>	getAllowMethodsDirective(std::list<std::string> token_list, Route *new_route);
		std::list<std::string>	getCgiPathDirective(std::list<std::string> token_list, Route *new_route);
		std::list<std::string>	getIndexDirective(std::list<std::string> token_list, Route *new_route);
		std::list<std::string>	getProxyPassDirective(std::list<std::string> token_list, Route *new_route);
		std::list<std::string>	getRootDirective(std::list<std::string> token_list, Route *new_route);
		std::list<std::string>	getNextServerDirective(std::list<std::string> token_list, Server *new_server);
		std::list<std::string>	getListenDirective(std::list<std::string> token_list, Server *new_server);
		std::list<std::string>	getHostDirective(std::list<std::string> token_list, Server *new_server);
		std::list<std::string>	getServerNameDirective(std::list<std::string> token_list, Server *new_server);
		std::list<std::string>	getErrorPageDirective(std::list<std::string> token_list, Server *new_server);
		std::list<std::string>	getRootDirective(std::list<std::string> token_list, Server *new_server);
		std::list<std::string>	getIndexDirective(std::list<std::string> token_list, Server *new_server);
		std::list<std::string>	getLocationDirective(std::list<std::string> token_list, Server *new_server);
		std::list<std::string>	getMaxBodySizeDirective(std::list<std::string> token_list);
		std::list<std::string>	getNextServerConfig(std::list<std::string> token_list);
		std::list<std::string>	getServerDirectives(std::list<std::string>	token_list);
		std::list<std::string>	tokenizeConfigurationFile(std::string filename);
		void					parseConfigurationFile(std::string filename);
};

#endif

