/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 14:13:39 by achansar          #+#    #+#             */
/*   Updated: 2024/02/19 15:36:39 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
# define ROUTE_HPP

# include <string>
# include <iostream>

class Route
{
	public:
		void redirectRequest();

	private:
		bool			_autoindex;
		bool			_cgi;
		bool			_delete;
		bool			_get;
		bool			_post;
		std::string		_index;
		std::string		_path;
		std::string		_redirection;
		std::string		_root;
};

#endif