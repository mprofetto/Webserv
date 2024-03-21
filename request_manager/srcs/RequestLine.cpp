/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 20:28:18 by nesdebie          #+#    #+#             */
/*   Updated: 2024/03/21 15:54:58 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RequestLine.hpp"

RequestLine::RequestLine() {}

RequestLine::RequestLine(int const &method, std::string const &path, std::string const &http_version, std::string const &methodName) : _method(method), _http_version(http_version), _query("") {
    if (_method == UNVALID)
        _not_valid = methodName;
    else
        _not_valid = "";

    size_t pos = path.find('?');
    if (pos == std::string::npos)
        _path = path;
    else {
        _path = path.substr(0, pos);
        _query = path.substr(pos + 1);
    }
}

RequestLine::RequestLine(const RequestLine & copy) {
    *this = copy;
}

RequestLine::~RequestLine() {
}


/* ----- GETTERS ----- */

int RequestLine::getMethod() const {
    return _method;
}

std::string RequestLine::getPath() const {
    return _path;
}

std::string RequestLine::getHTTPVersion() const {
    return _http_version;
}

std::string RequestLine::getNotValid() const {
    return _not_valid;
}

std::string RequestLine::getQuery() const {
    return _query;
}

/* ----- OPERATORS ----- */

RequestLine& RequestLine::operator=(RequestLine const &op) {
    if (this != &op) {
        _method = op._method;
        _path = op._path;
        _http_version = op._http_version;
        _not_valid = op._not_valid;
    }
    return *this;
}

std::ostream & operator<<(std::ostream &o, RequestLine const &obj) {
    std::string httpMethods[4] = {"DELETE", "GET", "POST", obj.getNotValid()};
	o << httpMethods[obj.getMethod()]<< ": " << obj.getPath() << " " << obj.getHTTPVersion() << std::endl;
    return o;
}
