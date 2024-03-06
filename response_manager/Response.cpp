/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:58:55 by achansar          #+#    #+#             */
/*   Updated: 2024/03/06 15:19:40 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <sstream>
#include <fstream>
#include <sys/socket.h>

// ============================================================================== CONSTRUCTORS

Response::Response(const int statusCode) : _statusCode(statusCode), _path("/") {
    return;
}

Response::~Response() {
    return;
}

// ============================================================================== MEMBER FUNCTIONS

std::string getReason(int sc) {//           how to get the right reason message ?

    std::map<int, std::string> reasons;
    reasons.insert(std::make_pair(200, "OK"));
    reasons.insert(std::make_pair(400, "Bad Request"));
    reasons.insert(std::make_pair(403, "Forbidden"));
    reasons.insert(std::make_pair(404, "Not Found"));
    reasons.insert(std::make_pair(500, "Internal Server Error"));

    std::map<int, std::string>::iterator it = reasons.find(sc);
    if (it == reasons.end()) {
        std::cout << "Status code not implemented !" << std::endl;
    }
    
    return it->second;
}

std::string getHeaders(const int s) {//          which header is important ?
    std::string h;

    h += "Content-Type: text/html\r\n";
    h += "Content-Length: " + std::to_string(s) + "\r\n";// virer tostirng
    return "";
}

std::string Response::getBody() {
    
	std::ifstream			myfile;
    std::string             line;
    std::string             body;

    myfile.open(_path);
	if (myfile.fail()) {
		std::cout << "ERROR OPENING" << std::endl;
    }

    while (std::getline(myfile, line)) {
        body += line;
    }

    myfile.close();
    return body;
}

void      Response::buildResponse(Request request) {
    
    // int method = request.getRequestLine().getMethod();
    // std::cout << "Method : " << method << std::endl;

    switch (request.getRequestLine().getMethod()) {
        case 1:
            buildGetResponse(request);
            break;
        case 2:
            buildPostResponse(request);
            break;
        case 0:// DELETE
            break;
        default:
            break;//    ?
    }
    return;
}

void Response::buildPostResponse(Request request) {
    (void)request;
    return;
}

void Response::buildGetResponse(Request request) {

    (void)request;
    std::stringstream   ss;
    std::string         headers;
    std::string         body;

    ss << _statusCode;

    _statusLine = "HTTP/1.0 " + ss.str() + " " + getReason(_statusCode) + "\n";

    // if (_body.size())
        body = getBody() /*+ "\n"*/;
    // if (_headers.size())
        headers = getHeaders(body.length()) + "\n";

    _responseLine = _statusLine + headers + body;

    // std::cout << "RESPONSE :\n" << _responseLine << std::endl;

    return;
}

// ============================================================================== GETTER & SETTER

std::string     Response::getResponse() {
    return _responseLine;
}

std::string     Response::getPath() {
    return _path;
}

void            Response::setPath(std::string& str) {
    _path = str;
    return;
}
