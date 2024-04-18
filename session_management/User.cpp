/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 10:14:12 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/17 12:15:17 by mprofett         ###   ########.fr       */
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
	this->_lang = copy.getLang();
	this->_articles = copy.getArticles();
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

std::string		User::getLang(void) const
{
	return (this->_lang);
}

std::list<std::string>	User::getArticles(void) const
{
	return (this->_articles);
}

void					User::setLang(std::string &lang)
{
	this->_lang = lang;
}

void			User::setSessionId(std::string &session_id)
{
	this->_session_id = session_id;
}

void			User::addArticles(std::string article)
{
	std::list<std::string>::iterator	it;

	it = this->_articles.begin();
	while(it != this->_articles.end())
	{
		if ((*it).compare(article) == 0)
			return;
		++it;
	}
	this->_articles.push_back(article);
}

void			User::removeArticles(std::string article)
{
	this->_articles.remove(article);
}

