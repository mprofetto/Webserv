/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 11:34:39 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/16 12:35:50 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
# define ROUTE_HPP

# include <string>

class Route
{
	public:

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
