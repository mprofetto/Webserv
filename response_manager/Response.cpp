/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:58:55 by achansar          #+#    #+#             */
/*   Updated: 2024/03/04 11:52:12 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <sstream>
#include <fstream>
#include <sys/socket.h>

// ============================================================================== CONSTRUCTORS

Response::Response(const int statusCode) : _statusCode(statusCode) {
    return;
}

Response::~Response() {
    return;
}

// ============================================================================== GETTER & SETTER

std::string     Response::getResponse() {
    return _responseLine;
}


// ============================================================================== MEMBER FUNCTIONS

std::string getReason() {//           how to get the right reason message ?
    return "OK";
}

std::string getHeaders(const int s) {//          which header is important ?
    std::string h;

    h += "Content-Type: text/plain\r\n";
    h += "Content-Length: " + std::to_string(s) + "\r\n";// virer tostirng
    return "";
}

std::string getBody() {
    
	std::ifstream			myfile;
    std::string             line;
    std::string             body;
    
    myfile.open("index/home.html");
	if (myfile.fail()) {
		std::cout << "ERROR OPENING" << std::endl;
    }

    while (std::getline(myfile, line)) {
        body += line;
    }

    myfile.close();
    return body;
}

void Response::getResponseLine() {

    std::stringstream   ss;
    std::string         headers;
    std::string         body;

    ss << _statusCode;

    _statusLine = "HTTP/1.0 " + ss.str() + " " + getReason() + "\n";
        
    // if (_body.size())
        body = getBody() /*+ "\n"*/;
    // if (_headers.size())
        headers = getHeaders(body.length()) + "\n";

    _responseLine = _statusLine + headers + body;

    // std::cout << "RESPONSE :\n" << _responseLine << std::endl;
    
    // ssize_t bytesSent = send(_clientSocket, _responseLine.c_str(), _responseLine.length(), 0);
    // if (bytesSent < 0) {
    //     perror("Error sending _responseLine");
    // } else {
    //     std::cout << "_responseLine sent successfully" << std::endl;
    // }

    return;
}
