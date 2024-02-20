/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 20:28:18 by nesdebie          #+#    #+#             */
/*   Updated: 2024/02/20 14:08:31 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RequestLine.hpp"

RequestLine::RequestLine() {}

RequestLine::RequestLine(const int& method, const std::string& path, const std::string& http_version, const std::string& methodName) : _method(method), _path(path), _http_version(http_version) {
    if (_method == UNVALID)
        _not_valid = methodName;
    else
        _not_valid = "";
    /*std::ifstream file(path); // TO EDIT
    if (!file.is_open())
        throw std::runtime_error("Error: data.csv not found.");*/
}

RequestLine::RequestLine(const RequestLine& copy){
    *this = copy;
}

RequestLine::~RequestLine() {}

RequestLine& RequestLine::operator=(const RequestLine& op) {
    if (this != &op) {
        _method = op._method;
        _path = op._path;
        _http_version = op._http_version;
        _not_valid = op._not_valid;
    }
    return *this;
}

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

std::ostream & operator<<(std::ostream & o, RequestLine const & obj)
{
    std::string httpMethods[4] = {"DELETE", "GET", "POST", obj.getNotValid()};
	o << httpMethods[obj.getMethod()]<< ": " << obj.getPath() << " " << obj.getHTTPVersion() << std::endl;
    return o;
}