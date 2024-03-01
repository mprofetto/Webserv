/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:58:55 by achansar          #+#    #+#             */
/*   Updated: 2024/03/01 12:23:59 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <sstream>
#include <fstream>

// ============================================================================== CONSTRUCTORS

Response::Response(const int statusCode) : _statusCode(statusCode) {
    return;
}

Response::~Response() {
    return;
}

// ============================================================================== MEMBER FUNCTIONS


std::string getReason() {//           how to get the right reason message ?
    return "OK";
}

std::string getHeaders() {//          which header is important ?
    return "";
}

std::string getBody() {//             what kind of body message do we need ? Is it necessary ?
    
	std::ifstream			myfile;
    std::string             line;
    std::string             body;
    
    myfile.open("../index/home.html");
	if (std::ios_base::failbit == true)
		std::cout << "ERROR OPENING" << std::endl;
    
    while (std::getline(myfile, line)) {
        body += line;
    }
    
    return line;
}

void Response::getResponseLine() {
    
    std::stringstream   ss;
    std::string         headers;
    std::string         body;
    
    ss << _statusCode;
    
    _statusLine = "HTTP/1.0 " + ss.str() + " " + getReason() + "\n";
        
    if (_headers.size())
        headers = getHeaders() + "\n";
    if (_body.size())
        body = getBody() /*+ "\n"*/;

    _responseLine = _statusLine + headers + body;

    std::cout << "RESPONSE :\n" << _responseLine << std::endl;
    return;
}
