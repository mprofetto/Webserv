/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 10:34:15 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/17 16:11:11 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

std::string	Server::extractInfoFromCookie(std::string &cookie, std::string info)
{
	size_t		start;
	size_t		end;
	std::string	result = "";
	std::string	sub_str = info + "=";

	start = header.find(sub_str);
	if (start != std::npos)
	{
		start += sub_str.size();
		end = header.find(";\n", start);
		result = header.substr(start, end - start + 1);
	}
	return (result);
}

std::string	Server::extractCookieFromHeader(std::string &header)
{
	size_t		start;
	size_t		end;
	std::string	result = "";
	std::string	sub_str = "Cookie: ";

	start = header.find(sub_str);
	if (start != std::npos)
	{
		end = header.find("\n", start);
		result = header.substr(start, end - start + 1);
	}
	return (result);
}

std::string	replaceUserName(std::string &body, std::string name, std::string word_to_replace)
{
	size_t	index;

	index = body.find(word_to_replace);
	if (index != std::npos)
		body.replace(index, word_to_replace.size(), name);
	return (body);
}

std::string	Server::applyCookieLang(std::string	&filename, std::string &lang)
{
	if (lang.compare("lang=fr") == 0)
	{
		index = filename.find(".html");
		filename.insert(index, "-fr");
	}
	return (filename);
}

std::string	Server::applyCookieNavInfo(std::string &sessionId, std::string &response_body)
{
	std::string	name;
	std::string	to_replace;
	User		user;

	if (session_id.empty() == true)
		return (body);
	user = getUserBySessionId(session_id);
	name = user.getName();
	if (name.empty() == false)
	{
		if (user.getLang.compare("lang=fr") == 0)
			body = replaceUserName(body, name, "invité");
		else
			body = replaceUserName(body, name, "guest");
	}
	return (body);
}

std::string	Server::generateSessionIdToken(std::string &sessionId, std::string &response_header, unsigned int duration)
{
	User		user;
	size_t		index;
	std::string	new_sessionId = generateSessionId();
	std::string	new_expirationDate = generateExpirationDate(duration);
	std::string	argument;

	user = getUserBySessionId(session_id);
	user.setSessionId(new_sessionId);
	argument = "Set-Cookie: id=" + new_sessionId + "; " + new_expirationDate + "\r\n";
	index = response_header.find("Content-Length:");
	response_header.insert(index, argument);
	return (response_header);
}

std::string	Server::setCookie(std::string &value, std::string &response_header)
{
	size_t		index;
	std::string	argument;

	argument = "Set-Cookie: " + value + "\r\n";
	index = response_header.find("Content-Length:");
	response_header.insert(index, argument);
	return (response_header);
}

/*
	SESSION RENWALL
	FOR Any Request
	Build Response
	std::string	cookie_infos = route->server->extractCookieFromHeader(_request.getHeader());
	std::string	id_info = route->server->extractInfoFromCookie(cookie_infos, "id");

	if (id_info.empty() == false)
	{
		std::string	session_id = id_info.substr(id_info.find_first_of("=") + 1, std::npos);
		_headers = route->server->renewSessionIdToken(session_id, _headers, COOKIE_EXPIRATION_DURATION);
	}
	Send Response
/*

/*
	LOGGIN
	WHEN Request is POST /loggin

	std::filename = "index.html";

	Build Response

	std::string		user;
	std::string		password;
	size_t			start_index;
	size_t			end_index;

	if (_method == POST && _path.compare("/loggin") == 0)
	{
		start_index = _request.getBody().find("=") + 1;
		end_index = _request.getBody().find("&");
		user = _request.getBody().substr(start_index, end_index - start_index + 1);
		start_index = _request.getBody().find("=", end_index + 1) + 1;
		password = _request.getBody().substr(start_index, std::npos);
		if (route->server->userIsValid(user, password))
		{
			_headers = route->server->renewSessionIdToken(session_id, _headers, COOKIE_EXPIRATION_DURATION);
		}
	}
	Send Response
*/

/*
	LOGGOUT
	WHEN Request is POST /loggout

	std::filename = "index.html";

	Build Response
	std::string	cookie_infos = route->server->extractCookieFromHeader(_request.getHeader());
	std::string	id_info = route->server->extractInfoFromCookie(cookie_infos, "id");

	if (id_info.empty() == false)
	{
		std::string	session_id = id_info.substr(id_info.find_first_of("=") + 1, std::npos);
		_headers = route->server->renewSessionIdToken(session_id, _headers, 0);
	}
	Send Response
*/

/*
	LANGAGE CHANGE
	WHEN Request is POST /lang

	std::string	filename;

	if (_method == POST && _path.compare("/lang") == 0)
	{
		if (_request.getBody().compare("lang=fr"))
			filename = index-fr.html;
		else
			filename = index.html;
	}
	Build Response
	_headers = route->server->setCookie(_request.getBody(), _headers)
	Send Response
*/

/*
	LANGAGE COOKIE CHECK
	WHEN we already have filename

	Get filename;
	std::string	cookie_infos = route->server->extractCookieFromHeader(_request.getHeader());
	std::string	lang_info = route->server->extractInfoFromCookie(cookie_infos, "lang");

	if (lang_info.empty() == false)
		filename = route->server->applyCookieLang(filename, lang_info);
	Build Response
	Send Response
*/

/*
	Get filename
	if CGI
		Nestor functions
	else
	{
		CHECK IF POST LANGAGE CHANGE/LOGGIN/LOGGOUT
		LANGAGE COOKIE CHECK
		Build Response
	}
	SESSION RENWALL
	Send Response
*/

// void	Response::checkLangageCookie(void)
// {
// 	std::string	cookie_infos = _server->extractCookieFromHeader(_request.getHeader());
// 	std::string	lang_info = _server->extractInfoFromCookie(cookie_infos, "lang");

// 	if (lang_info.empty() == false)
// 		_path = _server->applyCookieLang(filename, lang_info);
// }

// void	Response::replaceUserName(std::string name, std::string word_to_replace)
// {
// 	size_t	index;

// 	index = _path.find(word_to_replace);
// 	if (index != std::npos)
// 		_path.replace(index, word_to_replace.size(), name);
// }

// void	Response::checkLogCookie(void)
// {
// 	std::string	cookie_infos = _server->extractCookieFromHeader(_request.getHeader());
// 	std::string	id_info = _server->extractInfoFromCookie(cookie_infos, "id");

// 	if (id_info.empty() == false)
// 	{
// 		std::string	name;
// 		std::string	to_replace;
// 		std::string	lang_info = _server->extractInfoFromCookie(cookie_infos, "id");
// 		User		user;

// 		user = getUserBySessionId(id_info);
// 		name = user.getName();
// 		if (name.empty() == false)
// 		{
// 			if (lang_info("fr") == 0)
// 				replaceUserName(name, "invité");
// 			else
// 				replaceUserName(name, "guest");
// 		}
// 	}
// }

// void	Response::getFileContent(std::string filename)
// {
// 	std::ifstream	myfile;
// 	std::string		line;

// 	myfile.open(filename);
// 	if (myfile.fail()) {
// 		_statusCode = 500;
// 	}
// 	else {
// 		while (std::getline(myfile, line)) {
// 			_body += line;
// 		}
// 	}
// 	myfile.close();
// }

// bool	Response::bodyIsNeeded(void)
// {
// 	if (_method == GET)
// 		return (true);
// 	else if (_method == POST)
// 	{
// 		if ((_path.compare("/loggin") == 0)
// 			|| (_path.compare("/loggout") == 0)
// 			|| ((_path.compare("/lang") == 0 && _request.getBody().compare("lang=eng") == 0)))
// 			_path = "docs/index.html";
// 		else if (_path.compare("/lang") == 0 && _request.getBody().compare("lang=fr") == 0)
// 			_path = "docs/index-fr.html";
// 		else
// 			return (false);
// 		return (true);
// 	}
// 	return (false);
// }

// void	Response::getBody(bool autoindex) {

// 	if (bodyIsNeeded() == true) {
// 		if (isDirectory(_path)) {
// 			if (autoindex) {
// 				_statusCode = generateAutoindex();
// 			} else {
// 				_statusCode = 403;
// 			}
// 		} else {
// 			checkLangageCookie();
// 			getFileContent(_path);
// 			checkLogCookie();
// 		}
// 	}
// 	else {
// 		_body = "";
// 	}
// }

// void	Response::setCookieSession(void)
// {
// 	std::string		name;
// 	std::string		password;
// 	size_t			start_index;
// 	size_t			end_index;
// 	User			user;

// 	start_index = _request.getBody().find("=") + 1;
// 	end_index = _request.getBody().find("&");
// 	name = _request.getBody().substr(start_index, end_index - start_index + 1);
// 	start_index = _request.getBody().find("=", end_index + 1) + 1;
// 	password = _request.getBody().substr(start_index, std::npos);
// 	user = getUserByName(name);
// 	user.setSessionId(generateSessionId(COOKIE_EXPIRATION_DURATION));
// 	if (_server->userIsValid(name, password))
// 		_headers = _server->generateSessionIdToken(user.getSessionId(), _headers, generateSessionId(COOKIE_EXPIRATION_DURATION));
// }

// void	Response::closeCookieSession(void)
// {
// 	std::string	cookie_infos = _server->extractCookieFromHeader(_request.getHeader());
// 	std::string	id_info = _server->extractInfoFromCookie(cookie_infos, "id");

// 	if (id_info.empty() == false)
// 	{
// 		std::string	session_id = id_info.substr(id_info.find_first_of("=") + 1, std::npos);
// 		_headers = _server->generateSessionIdToken(session_id, _headers, 0);
// 	}
// }

// void	Response::renewCookieSession(void)
// {
// 	std::string	cookie_infos = _server->extractCookieFromHeader(_request.getHeader());
// 	std::string	id_info = _server->extractInfoFromCookie(cookie_infos, "id");

// 	if (id_info.empty() == false)
// 	{
// 		std::string	session_id = id_info.substr(id_info.find_first_of("=") + 1, std::npos);
// 		_headers = _server->generateSessionIdToken(session_id, _headers, COOKIE_EXPIRATION_DURATION);
// 	}
// }

// std::string Response::getHeaders(const int s) {
//     std::string h;

//     if (_method == GET) {
//         h += "Content-Type: " + getMimeType() + "\r\n";
//     }
//     h += "Content-Length: " + std::to_string(s) + "\r\n";
// 	if (_method == POST) {
// 		if (_path.compare("/lang") == 0)
// 			_headers = route->server->setCookie(_request.getBody(), _headers)
// 		else if (_path.compare("/loggin") == 0)
// 			setCookieSession();
// 		else if (_path.compare("/loggout") == 0)
// 			closeCookieSession();
// 	}
// 	renewCookieSession();
//     return h;
// }
