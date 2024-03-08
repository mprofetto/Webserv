/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorResponse.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 16:09:15 by achansar          #+#    #+#             */
/*   Updated: 2024/03/08 17:17:51 by achansar         ###   ########.fr       */
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

void    Response::buildErrorResponse() {
    
    std::stringstream   ss;
	std::ifstream			myfile;
    std::string             line;
        
    std::map<int, std::string> errorMap = _server->getErrorPages();
    std::map<int, std::string>::iterator it = errorMap.find(_statusCode);
    if (it != _server->getErrorPages().end()) {
        // myfile.open(it->second);
        myfile.open("docs/index.html");
    } else if (it != _server->getErrorPages().end() || myfile.fail()) {
        _statusCode = 500;//           should also return a 500.html
        _body = get500ErrorPage();//    or a hardcoded 500 html function ?
        return;
    }

    while (std::getline(myfile, line)) {
        _body += line;
    }
    myfile.close();

    _headers = getHeaders(_body.length());
    ss << _statusCode;
    _statusLine = "HTTP/1.0 " + ss.str() + " " + getReason(_statusCode) + "\n";
    return;
}
