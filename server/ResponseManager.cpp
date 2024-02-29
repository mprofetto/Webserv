/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseManager.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:58:55 by achansar          #+#    #+#             */
/*   Updated: 2024/02/19 17:31:34 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseManager.hpp"
#include <sstream>

// ============================================================================== CONSTRUCTORS

ResponseManager::ResponseManager(const int statusCode) : _statusCode(statusCode) {
    return;
}

ResponseManager::~ResponseManager() {
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
    return "";
}

void ResponseManager::getResponseLine() {
    
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
    return;
}