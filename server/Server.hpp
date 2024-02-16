/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 11:15:25 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/16 13:15:50 by mprofett         ###   ########.fr       */
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
