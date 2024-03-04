/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 11:12:53 by nesdebie          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/02/28 17:23:40 by nesdebie         ###   ########.fr       */
=======
/*   Updated: 2024/02/29 14:26:40 by nesdebie         ###   ########.fr       */
>>>>>>> 333fa626db11e8f056a3eabc8aa985f52339e39d
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

<<<<<<< HEAD
Request::Request(std::string & req): _raw(req){
    _body = "";
=======
Request::Request(std::string & req): _raw(req), _body("") {
>>>>>>> 333fa626db11e8f056a3eabc8aa985f52339e39d
    std::istringstream          iss(req);
    std::string                 line;
    int                         count = 0;

    while (std::getline(iss, line, '\n')) {
        if (strlen(line.c_str()) == 0)
            continue ;
        if (count == 0) {
            std::vector<std::string> arr = vectorSplit(line, SPACE);
            std::string httpMethods[3] = {"DELETE", "GET", "POST"};
            int method;

            for (method = 0; method < 3 && httpMethods[method] != arr[0]; method++);
            RequestLine reqline(method, arr[1], arr[2], arr[0]);
            _req = reqline;
            count++;
            continue ;
        }
        if (_body != "") {
            _body += "\n"; //INUTILE, juste plus facile à lire
            _body += line;
            continue ;
        }
        size_t pos = line.find(':');
        if (pos == std::string::npos && _req.getMethod() != GET) {
            _body = line;
            continue ;
        }
        std::string headerName = line.substr(0, pos);
        std::string headerVal = line.substr(pos + 1).c_str();
        this->setData(headerName, ft_strtrim(headerVal));
    }
}

Request::Request(Request const &copy) {
    *this = copy;
}

Request::~Request() {
}


/* ----- CLASS FUNCTIONS ----- */

std::vector<std::string> Request::vectorSplit(std::string str, char sep) {
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

std::string Request::ft_strtrim(std::string &s) {
    s.erase(s.find_last_not_of(" \t\n\r") + 1);
    s.erase(0, s.find_first_not_of(" \t\n\r"));
    return s;
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

std::string Request::getHeader(std::string const &name) {
<<<<<<< HEAD
    if (this->_headers.size() == 0)
        return 0;
=======
    if (_headers.size() == 0)
        throw HeaderNotFoundException();
>>>>>>> 333fa626db11e8f056a3eabc8aa985f52339e39d
    std::map<std::string, std::string>::iterator it = this->_headers.find(name);
    if (it->first != name)
        throw HeaderNotFoundException();
    return it->second;
}


/* ----- OPERATORS ----- */

Request & Request::operator=(Request const &op) {
    _raw = op._raw;
    _req = op._req;
    _headers = op._headers;
    _body = op._body;
    return *this;
}

std::ostream & operator<<(std::ostream &o, Request const &obj) {
	o << obj.getRequestLine();
    std::map<std::string, std::string> headers = obj.getHeaders();
    std::map<std::string, std::string>::iterator it = headers.begin();
    for (unsigned long i = 0; i < headers.size(); i++){
        o << it->first << ": " << it->second << std::endl;
        it++;
    }
    if (strlen(obj.getBody().c_str()))
        o << std::endl << obj.getBody();
    return o;
}


/* ----- EXCEPTIONS ----- */

const   char* Request::HeaderNotFoundException::what() const throw() {
    return "Header Not Found";
}

/*
int main() {
    try {
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

        std::string str4 = "POST /cgi-bin/process.cgi HTTP/1.1\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\nHost: www.tutorialspoint.com\nContent-Type: application/x-www-form-urlencoded\nContent-Length: length\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive\n\nlicenseID=string&content=string&/paramsXML=string\n{\n[BODY EXAMPLE]\n}";
        Request obj4(str4);
        std::cout << obj4 << std::endl << std::endl;
    } catch (std::exception const & e) {
        std::cerr << e.what() << std::endl;
    }
}*/