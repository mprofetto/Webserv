/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 14:05:51 by achansar          #+#    #+#             */
/*   Updated: 2024/02/19 14:14:44 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestManager.hpp"

RequestManager::RequestManager(std::string & req): _raw(req){ // TEMP 
    _body = "";
    std::istringstream          iss(req);
    std::string                 line;
    int                         count = 0;

    while (std::getline(iss, line, '\n'))
    {
        if (strlen(line.c_str()) == 0)
            continue ;
        if (count == 0) {
            try {
                std::vector<std::string> arr = vectorSplit(line, SPACE);
                std::string httpMethods[4] = {"DELETE", "GET", "POST", "PUT"}; //put not mandatory
                int method;

                for (method = 0; method < 4 && httpMethods[method] != arr[0]; method++);
                RequestLine reqline(method, arr[1], arr[2]);
                _req = reqline;
                count++;
                continue ;
            } catch (const std::exception & e) {
                throw std::runtime_error("ERROR");
            }
        }
        size_t pos = line.find(':');
        if (pos == std::string::npos) { // ajouter des IF selon methode
            _body = line;
            continue ;
        } // mettre le reste dans else quand nouvelles conditions if
        std::string headerName = line.substr(0, pos);
        std::string headerVal = line.substr(pos + 1).c_str();
        this->setData(headerName, ft_strtrim(headerVal));
    }
}
/*
RequestManager::RequestManager(std::string & req, Route & route, Server & server): _raw(req){ 
    (void) route;
    (void) server;
    
    _body = ""; //a changer cas particuliers
    std::istringstream          iss(req);
    std::string                 line;
    int                         count = 0;

    while (std::getline(iss, line, '\n'))
    {
        if (strlen(line.c_str()) == 0)
            continue ;
        if (count == 0) {
            try {
                std::vector<std::string> arr = vectorSplit(line, SPACE);
                std::string httpMethods[4] = {"DELETE", "GET", "POST", "PUT"};
                int method;

                for (method = 0; method < 4 && httpMethods[method] != arr[0]; method++);
                RequestLine reqline(method, arr[1], arr[2]);
                _req = reqline;
                count++;
                continue ;
            } catch (const std::exception & e) {
                throw std::runtime_error("ERROR");
            }
        }
        size_t pos = line.find(':');
        if (pos == std::string::npos) {
            _body = line;
            continue ;
        }
        std::string headerName = line.substr(0, pos);
        std::string headerVal = line.substr(pos + 1).c_str();
        this->setData(headerName, ft_strtrim(headerVal));
    }
}

RequestManager::RequestManager(RequestManager const & copy) {
    *this = copy;
}

RequestManager::~RequestManager() {
}

RequestManager & RequestManager::operator=(RequestManager const & op) {
    _raw = op._raw;
    _req = op._req;
    _headers = op._headers;
    _body = op._body;
    return *this;
}

std::vector<std::string> RequestManager::vectorSplit(std::string str, char sep){
    std::vector<std::string> arr;
    char* cstr = const_cast<char*>(str.c_str());
    char* token = std::strtok(cstr, &sep);
    int count = 0;
    while (token != 0) {
        arr.push_back(token);
        token = std::strtok(0, &sep);
        count++;
    }
    return arr;
}*/

void RequestManager::setData(std::string head, std::string val) {
    this->_headers.insert(std::make_pair(head, val));
}

std::string RequestManager::ft_strtrim(std::string & s) {
    s.erase(s.find_last_not_of(" \t\n\r") + 1);
    s.erase(0, s.find_first_not_of(" \t\n\r"));
    return s;
}

std::ostream & operator<<(std::ostream & o, RequestManager const & obj)
{
	o << obj.getRequestLine();
    std::map<std::string, std::string> headers = obj.getHeaders();
    std::map<std::string, std::string>::iterator it = headers.begin();
    for (int i = 0; i < headers.size(); i++){
        o << it->first << ": " << it->second << std::endl;
        it++;
    }
    if (strlen(obj.getBody().c_str()))
        o << std::endl << obj.getBody();
    return o;
}


/* ----- GETTERS ----- */

std::string RequestManager::getRaw() const {
    return _raw;
}

RequestLine RequestManager::getRequestLine() const {
    return _req;
}

std::string RequestManager::getBody() const {
    return _body;
}

std::map<std::string, std::string> RequestManager::getHeaders() const {
    return _headers;
}

std::string RequestManager::getHeader(std::string const & name) {
    std::map<std::string, std::string>::iterator it = this->_headers.find(name);
    if (it->first != name)
        return 0;
    return it->second;
}