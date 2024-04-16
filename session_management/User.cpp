/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 10:14:12 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/12 11:44:12 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User() : _name(""), _password(""), _session_id(""), _session_expiration_date(0)
{
	return;
}

User::User(std::string name, std::string password, std::string session_id, time_t session_expiration_time) :
	_name(name),
	_password(password),
	_session_id(session_id),
	_session_expiration_date(session_expiration_time)
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
	this->_session_expiration_date = copy.getSessionExpirationDate();
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

time_t			User::getSessionExpirationDate(void) const
{
	return (this->_session_expiration_date);
}

void			User::setSessionId(std::string &session_id)
{
	this->_session_id = session_id;
}

void			User::setSessionExpirationDate(time_t &session_expiration_date)
{
	this->_session_expiration_date = session_expiration_date;
}

bool			User::sessionExpired(time_t &actualDate) const
{
	if (this->_session_expiration_date >= actualDate)
		return (true);
	return (false);
}
