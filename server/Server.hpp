/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 11:15:25 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/12 11:29:17 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sstream>
# include <string>
# include <list>
# include <map>
# include <vector>
# include <cstdlib>
# include <unistd.h>
# include "Route.hpp"
# include "../utils.hpp"
# include "../session_management/User.hpp"

# define LOCALHOST "127.0.0.1"

class Route;

class Server
{
	public:
		class	invalidPortRange : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	invalidErrorPageCode : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	invalidErrorPagePath : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	invalidHostFormat : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	invalidSocket : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		Server();
		~Server();

		std::list<Route *>			getRoute(void) const;
		std::list<std::string>		getServerNames(void) const;
		std::map<int, std::string>	getErrorPages(void) const;
		std::string					getDefaultErrorPage(int error_code) const;
		std::list<std::string>		getIndex(void) const;
		std::string					getHost(void) const;
		int							getPort(void) const;
		std::string					getRoot(void) const;
		int							getSocket(void) const;
		std::string					getMimeType(std::string ext) const;

		void						addRoute(Route *route);
		void						addServerName(std::string name);
		void						addErrorPage(int code, std::string path);
		void						addIndex(std::string filename);
		void						setHost(std::string path);
		void						setPort(int port);
		void						setRoot(std::string path);
		void						setSocket(int socket);

		// User Management

		bool						userIsValid(std::string name, std::string password);
		User						getUserByName(std::string name);
		User						getUserBySessionId(std::string name);

		void						addUser(User &new_user);

		// Route 						*getRoute(std::string path);

		std::map<std::string, std::string>	buildMimeTypes();
		bool								isServerName(std::string name) const;
		std::string							convertIpAddress(std::vector<std::string> address);
		void								printDatas(void) const;
		std::list<Route *>					_routes;

	private:
		std::list<std::string>				_server_names;
		std::list<std::string>				_index;
		std::list<User>						_users;
		std::map<int, std::string>			_error_pages;
		std::map<std::string, std::string>	_mime_types;
		std::string							_host;
		std::string							_root;
		int									_port;
		int									_socket;
};

#endif
