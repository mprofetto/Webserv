/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorResponse.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 16:09:15 by achansar          #+#    #+#             */
/*   Updated: 2024/04/18 14:56:49 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <filesystem>

std::string get500ErrorPage() {//     TEMPORARY.... or not ?
    std::string page = "<!DOCTYPE html>\n";
    page += "<html>\n";
    page += "<head>\n";
    page += "<title>500 Internal Server Error</title>\n";
    page += "</head>\n";
    page += "<body>\n";
    page += "<h1>500 Internal Server Error</h1>\n";
    page += "<p>Sorry, something went wrong on the server.</p>\n";
    page += "</body>\n";
    page += "</html>\n";
    return page;
}

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
    

    std::cout << "Before redirect, statusCode is : " << _statusCode << std::endl;
    if (_statusCode == 301) {
        redirectClient();
        return;
    }
    std::map<int, std::string> errorMap = _server->getErrorPages();
    std::map<int, std::string>::iterator it = errorMap.find(_statusCode);
    if (it != _server->getErrorPages().end() && it != errorMap.end()) {
        myfile.open(it->second);
    } else if (it != _server->getErrorPages().end() || myfile.fail()) {
        _statusCode = 500;
        _body = get500ErrorPage();
        return;
    }

    while (std::getline(myfile, line)) {
        _body += line;
    }
    myfile.close();

    std::ostringstream intss;
    intss << _body.size();

    _headers = "Content-Type: text/html\r\n";
    _headers += "Content-Length: " + intss.str() + "\r\n\r\n";
    ss << _statusCode;
    _statusLine = "HTTP/1.1 " + ss.str() + " " + getReason(_statusCode) + "\r\n";
    return;
}
