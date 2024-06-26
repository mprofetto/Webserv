/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 09:46:07 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/29 18:11:49 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"

Route::Route() : _autoindex(true), _cgi(false), _delete(false), _get(false), _post(false), _extension(""), _path("/"), _redirection(""), _root("/")
{
	return;
}

Route::Route(Server *server) : _autoindex(true), _cgi(false), _delete(false), _get(false), _post(false), _extension(""), _path("/"), _redirection("")
{
	this->_server = server;
	return;
}

Route::~Route()
{
	return;
}

Server*					Route::getServer(void) const 
{
	return (this->_server);
}

bool					Route::getAutoindex(void) const
{
	return (this->_autoindex);
}

bool					Route::getCgi(void) const
{
	return (this->_cgi);
}

bool					Route::getDelete(void) const
{
	return (this->_delete);
}

bool					Route::getGet(void) const
{
	return (this->_get);
}

bool					Route::getPost(void) const
{
	return (this->_post);
}

std::string				Route::getExtension(void) const
{
	return (this->_extension);
}

std::list<std::string>	Route::getIndex(void) const
{
	return (this->_index);
}

std::string				Route::getPath(void) const
{
	return (this->_path);
}

std::string				Route::getRedirection(void) const
{
	return (this->_redirection);
}

std::string				Route::getRoot(void) const
{
	return (this->_root);
}

void					Route::addIndex(std::string filename)
{
	this->_index.push_back(filename);
}

void					Route::setAutoindex(bool value)
{
	this->_autoindex = value;
}

void					Route::setCgi(bool value)
{
	this->_cgi = value;
}

void					Route::setDelete(bool value)
{
	this->_delete = value;
}

void					Route::setGet(bool value)
{
	this->_get = value;
}

void					Route::setPost(bool value)
{
	this->_post = value;
}

void					Route::setExtension(std::string extension)
{
	this->_extension = extension;
}

void					Route::setPath(std::string path)
{
	this->_path = path;
}

void					Route::setRedirection(std::string url)
{
	this->_redirection = url;
}

void					Route::setRoot(std::string path)
{
	this->_root = path;
}

void					Route::printRoute(void) const
{
	std::list<std::string>::const_iterator		it_index;

	if (this->_extension.empty() == true)
		std::cout << "Route for path: " << this->_path << "\n";
	else
	{
		std::cout << "Route for extension: " << this->_extension << "\n";
		std::cout << "Extension needs cgi: " << this->_cgi << "\n";
	}
	if (this->_root.empty() == false)
		std::cout << "Root is: " << this->_root << "\n";
	else
		std::cout << "No root defined\n";
	std::cout << "Autoindex allowed: " << this->_autoindex << "\n";
	std::cout << "Get method allowed: " << this->_get << "\n";
	std::cout << "Delete method allowed: " << this->_delete << "\n";
	std::cout << "Post method allowed: " << this->_post << "\n";
	std::cout << "CGI : " << this->_cgi << "\n";
	it_index = this->_index.begin();
	std::cout << "Index files defined for this route: ";
	while (it_index != this->_index.end())
	{
		std::cout << *it_index;
		it_index++;
	}
	std::cout << "\n";
}
