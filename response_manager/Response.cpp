/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:58:55 by achansar          #+#    #+#             */
/*   Updated: 2024/03/19 14:44:46 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <filesystem>

// ============================================================================== CONSTRUCTORS

Response::Response(Server* server, int statusCode, const int method) :
        _method(method),
        _statusCode(statusCode),
        _path("/"),
        _server(server) {
    return;
}

Response::~Response() {
    return;
}

// ============================================================================== MEMBER FUNCTIONS

// ==================================================================== POST METHOD

void Response::buildPostResponse(Request request) {
    (void)request;
    return;
}

// ==================================================================== GET METHOD

std::string Response::getReason(int sc) {
//     the map better be part of the server object

    std::map<int, std::string> reasons;
    reasons.insert(std::make_pair(200, "OK"));
    reasons.insert(std::make_pair(201, "Created"));
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

std::string     Response::getMimeType() {

    std::map<std::string, std::string> MIMEtypes;
    MIMEtypes.insert(std::make_pair(".html", "text/html"));
    MIMEtypes.insert(std::make_pair(".txt", "text/plain"));
    MIMEtypes.insert(std::make_pair(".jpg", "image/jpeg"));
    MIMEtypes.insert(std::make_pair(".jpeg", "image/jpeg"));
    MIMEtypes.insert(std::make_pair(".png", "image/png"));
    MIMEtypes.insert(std::make_pair(".pdf", "application/pdf"));
    MIMEtypes.insert(std::make_pair("default", "text/html"));

    size_t extPos = _path.find_last_of(".");
    if (extPos != std::string::npos) {
        std::string extension = _path.substr(extPos, std::string::npos);

        std::map<std::string, std::string>:: iterator it = MIMEtypes.find(extension);
        if (it != MIMEtypes.end()) {
            return it->second;
        }
    }
    return MIMEtypes["default"];
}

std::string Response::getHeaders(const int s) {//          which header is important ?
    std::string h;

    // h += "Content-Type: text/html\r\n";//             get MIME type
    if (_method == GET) {
        h += "Content-Type: " + getMimeType() + "\r\n";
    }
    h += "Content-Length: " + std::to_string(s) + "\r\n";// virer tostirng
    return h;
}

std::string Response::getBody() {

	std::ifstream			myfile;
    std::string             line;
    std::string             body;

    if (_method == GET) {
        myfile.open(_path);
        if (myfile.fail()) {
            _statusCode = 500;
        }

        while (std::getline(myfile, line)) {
            body += line;
        }

        myfile.close();
    } else if (_method == POST) {
        body = "";
        // might be infos :
        // resource creation & confirmation message
    } else if (_method == DELETE) {
        body = "";
    }
    return body;
}

// finish get response + redirections 3XX (cf "Should I include a slash (/) at the end of my URLs?")
// start delete response

// void Response::buildGetResponse(Request request) {

//     (void)request;
//     std::stringstream   ss;
//     std::string         headers;
//     std::string         body;

//     ss << _statusCode;

//     _statusLine = "HTTP/1.0 " + ss.str() + " " + getReason(_statusCode) + "\n";
//     body = getBody(request.getRequestLine().getMethod());
//     headers = getHeaders(body.length()) + "\n";

//     _responseLine = _statusLine + headers + body;
//     return;
// }

// ==================================================================== SWITCH


void      Response::buildResponse(Request request) {

    std::stringstream   ss;
    (void)request;
    // int yes = 5;
    if (_statusCode == 200) {        
        // switch (request.getRequestLine().getMethod()) {
        //     case 1:
        //         buildGetResponse(request);
        //         break;
        //     case 2:
        //         buildPostResponse(request);
        //         break;
        //     case 0:// DELETE
        //         break;
        //     default:
        //         break;//    ?
        // }
        _body = getBody();
        _headers = getHeaders(_body.length()) + "\n";

        ss << _statusCode;
        _statusLine = "HTTP/1.0 " + ss.str() + " " + getReason(_statusCode) + "\n";
    } else {
        buildErrorResponse();
    }
    _responseLine = _statusLine + _headers + _body;
    return;
}

// ============================================================================== GETTER & SETTER

std::string     Response::getResponse() {
    return _responseLine;
}

std::string     Response::getPath() {
    return _path;
}

int             Response::getStatusCode() {
    return _statusCode;
}

void            Response::setPath(std::string& str) {
    _path = str;
    return;
}

void            Response::setErrorPath(std::string& str) {
    _errorPath = str;
    return;
}
