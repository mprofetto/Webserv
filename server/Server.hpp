/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 11:15:25 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/21 14:43:07 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <list>
# include <map>
# include "Route.hpp"

# define LOCALHOST "127.0.0.1"

class Server
{
	public:
		class	invalidPortRange : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		Server();
		~Server();

		std::list<Route>			getRoute(void) const;
		std::list<std::string>		getServerNames(void) const;
		std::map<int, std::string>	getErrorPages(void) const;
		std::string					getIndex(void) const;
		std::string					getIpAdress(void) const;
		int							getPort(void) const;
		std::string					getRoot(void) const;

		void						addRoute(Route route);
		void						addServerName(std::string name);
		void						addErrorPage(int code, std::string path);
		void						setIndex(std::string filename);
		void						setIpAdress(std::string ip);
		void						setPort(int port);
		void						setRoot(std::string path);

	private:
		std::list<Route>			_routes;
		std::list<std::string>		_server_names;
		std::map<int, std::string>	_error_pages;
		std::string					_index;
		std::string					_ipAdress;
		std::string					_root;
		int							_port;
};

#endif
