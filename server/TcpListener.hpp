/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpListener.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 09:32:35 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/21 14:27:40 by mprofett         ###   ########.fr       */
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
# include "../utils.hpp"

# define MAXBUFFERSIZE 2097152

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
		TcpListener(const char * ipAdress, int port, int buffer_max);
		~TcpListener();

		void	init();
		void	run();
		void	parseConfigurationFile(std::string filename);


	private:
		const char *_ipAdress; //temporary var
		int			_port; //temporary var

		long long int				_buffer_max;
		int							_socket;
		fd_set						_read_master_fd;
		fd_set						_write_master_fd;
		std::map<int, std::string>	_responses;
		std::list<Server *>			_servers;

		//parse config file utils
		void					getMaxBodySizeConfig(std::string arg);
		std::list<std::string>	getNextDirective(std::list<std::string> token_list, Server *new_server);
		std::list<std::string>	getNextServerConfig(std::list<std::string> token_list);
		std::list<std::string>	getServerDirectives(std::list<std::string>	token_list);
		// void					parseConfigurationFile(std::string filename);
		std::list<std::string>	tokenizeConfigurationFile(std::string filename);

		//init servers utils
		void			bindSocket();

		//listening request utils
		void			handleNewConnection();
		void			readRequest(int socket);
		void			writeResponse(int socket);
		int				getPortFromSocket(int *socket);
};

#endif

