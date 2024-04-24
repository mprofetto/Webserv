/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorResponse.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/04/19 16:12:45 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Response.hpp"
#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <filesystem>

void    Response::redirectClient() {

    std::stringstream response;
    response << "HTTP/1.1 301 Moved Permanently\r\n";
    response << "Location: " << _path << "\r\n\r\n";
    _statusLine = response.str().c_str();
    std::cout << "In redirect, LINE IS : " << _statusLine << std::endl;
    return;
}

void    Response::buildErrorResponse() {

    std::stringstream   ss;
	std::ifstream			myfile;
    std::string             line;

    if (_statusCode == 301) {
        redirectClient();
        return;
    }
    std::map<int, std::string> errorMap = _server->getErrorPages();
    std::map<int, std::string>::iterator it = errorMap.find(_statusCode);
    if (it != _server->getErrorPages().end() && it != errorMap.end()) {
        myfile.open(it->second.c_str());
        if (myfile.fail()) {
            _body = _server->getDefaultErrorPage(500);
        }
        else {
            while (std::getline(myfile, line)) {
                _body += line;
            }
            myfile.close();
        }
    } else if (it != _server->getErrorPages().end() || myfile.fail()) {
        _body = _server->getDefaultErrorPage(_statusCode);
    }

    std::ostringstream intss;
    intss << _body.size();

    _headers = "Content-Type: text/html\r\n";
    _headers += "Content-Length: " + intss.str() + "\r\n\r\n";
    ss << _statusCode;
    _statusLine = "HTTP/1.1 " + ss.str() + " " + getReason(_statusCode) + "\r\n";
    return;
}
