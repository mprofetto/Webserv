/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 14:14:05 by achansar          #+#    #+#             */
/*   Updated: 2024/02/19 14:14:06 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <list>
# include "Route.hpp"

class Server
{
	public:

	private:
		std::list<Route>		_routes;
		std::list<std::string>	_server_names;
		std::string				_index;
		std::string				_ipAdress;
		std::string				_root;
		int						_port;
};

#endif