/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 11:12:53 by nesdebie          #+#    #+#             */
/*   Updated: 2024/02/20 14:09:42 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

Request::Request(std::string & req): _raw(req){ // TEMP 
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
                std::string httpMethods[3] = {"DELETE", "GET", "POST"}; //put not mandatory
                int method;

                for (method = 0; method < 3 && httpMethods[method] != arr[0]; method++);
                RequestLine reqline(method, arr[1], arr[2], arr[0]);
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
Request::Request(std::string & req, Route & route, Server & server): _raw(req){ 
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
}*/

Request::Request(Request const & copy) {
    *this = copy;
}

Request::~Request() {
}

Request & Request::operator=(Request const & op) {
    _raw = op._raw;
    _req = op._req;
    _headers = op._headers;
    _body = op._body;
    return *this;
}

std::vector<std::string> Request::vectorSplit(std::string str, char sep){
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
}

void Request::setData(std::string head, std::string val) {
    this->_headers.insert(std::make_pair(head, val));
}

std::string Request::ft_strtrim(std::string & s) {
    s.erase(s.find_last_not_of(" \t\n\r") + 1);
    s.erase(0, s.find_first_not_of(" \t\n\r"));
    return s;
}

std::ostream & operator<<(std::ostream & o, Request const & obj)
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

std::string Request::getRaw() const {
    return _raw;
}

RequestLine Request::getRequestLine() const {
    return _req;
}

std::string Request::getBody() const {
    return _body;
}

std::map<std::string, std::string> Request::getHeaders() const {
    return _headers;
}

std::string Request::getHeader(std::string const & name) {
    std::map<std::string, std::string>::iterator it = this->_headers.find(name);
    if (it->first != name)
        return 0;
    return it->second;
}


// DEBUG
/*
int main() {
    std::string str0 = "GET / HTTP/1.1";
    Request obj0(str0);
    std::cout << obj0 << std::endl << std::endl;

    std::string str = "GET / HTTP/1.1\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\nHost: developer.mozilla.org\nAccept-Language: fr";
    Request obj(str);
    std::cout << obj << std::endl << std::endl;

    std::string str2 = "GET /hello.htm HTTP/1.1\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\nHost: www.tutorialspoint.com\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive";
    Request obj2(str2);
    std::cout << obj2 << std::endl << std::endl;

    std::string str3 = "POST /cgi-bin/process.cgi HTTP/1.1\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\nHost: www.tutorialspoint.com\nContent-Type: application/x-www-form-urlencoded\nContent-Length: length\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive\n\nlicenseID=string&content=string&/paramsXML=string";
    Request obj3(str3);
    std::cout << obj3 << std::endl << std::endl;
}
*/