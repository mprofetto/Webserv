/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 10:14:12 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/16 14:46:07 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User() : _name(""), _password(""), _session_id("")
{
	return;
}

User::User(std::string name, std::string password, std::string session_id) :
	_name(name),
	_password(password),
	_session_id(session_id)
{
	return;
}

User::~User()
{
	return;
}

User			&User::operator=(User &copy)
{
	this->_name = copy.getName();
	this->_password = copy.getPassword();
	this->_session_id = copy.getSessionId();
	return (*this);
}

std::string		User::getName(void) const
{
	return (this->_name);
}

std::string		User::getPassword(void) const
{
	return (this->_password);
}

std::string		User::getSessionId(void) const
{
	return (this->_session_id);
}

void			User::setSessionId(std::string &session_id)
{
	this->_session_id = session_id;
}
