/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_Users.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 10:54:14 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/12 11:45:29 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool	Server::userIsValid(std::string name, std::string password)
{
	User	user = this->getUserByName(name);

	if (user.getPassword() == password)
		return (true);
	return (false);
}

User	Server::getUserByName(std::string name)
{
	std::list<User>::iterator	it;
	User	result;

	it = this->_users.begin();
	while (it != this->_users.end())
	{
		if (name.compare((*it).getName()) == 0)
			return (*it);
		it++;
	}
	return (result);
}

User	Server::getUserBySessionId(std::string sessionId)
{
	std::list<User>::iterator	it;
	User	result;

	it = this->_users.begin();
	while (it != this->_users.end())
	{
		if (sessionId.compare((*it).getSessionId()) == 0)
			return (*it);
	}
	return (result);
}

void	Server::addUser(User &new_user)
{
	this->_users.push_back(new_user);
}
