/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 11:12:53 by nesdebie          #+#    #+#             */
/*   Updated: 2024/03/07 13:49:57 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

Request::Request() {
}

Request::Request(std::string & req): _raw(req){
    _body = "";
    _complete = true;
    _content_length = 0;

    _readRequest(req);

    if (_req.getMethod() == POST && strlen(getHeader("Content-Length").c_str())) {
        _content_length = atoi(getHeader("Content-Length").c_str());
        if (_content_length > CONTENT_LENGTH_MAX)
            throw ContentLengthException();
        if (_content_length < strlen(_body.c_str()))
            _complete = false;
    }
}

Request::Request(Request const &copy) {
    *this = copy;
}

Request::~Request() {
}


/* ----- PRIVATE FUNCTIONS ----- */

void Request::_readRequest(std::string const & request) {
    std::istringstream          iss(request);
    std::string                 line;
    int                         count = 0;
    while (std::getline(iss, line, '\n')) {
            if (strlen(line.c_str()) == 0)
                continue ;
            if (count == 0) {
                std::vector<std::string> arr = _vectorSplit(line, SPACE);
                std::string httpMethods[3] = {"DELETE", "GET", "POST"};
                int method;

                for (method = 0; method < 3 && httpMethods[method] != arr[0]; method++);
                _req =  RequestLine(method, arr[1], arr[2], arr[0]);
                count++;
                continue ;
            }
            if (_body != "") {
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
            this->_setData(headerName, _strtrim(headerVal));
        }
}

std::vector<std::string> Request::_vectorSplit(std::string str, char sep) {
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

void Request::_setData(std::string head, std::string val) {
    this->_headers.insert(std::make_pair(head, val));
}

std::string Request::_strtrim(std::string &s) {
    s.erase(s.find_last_not_of(" \t\n\r") + 1);
    s.erase(0, s.find_first_not_of(" \t\n\r"));
    return s;
}

/* ----- PUBLIC FUNCTIONS ----- */

void Request::catToBody(std::string & str) {
    _body += str;
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
    if (this->_headers.size() == 0)
        return "";
    std::map<std::string, std::string>::iterator it = this->_headers.find(name);
    if (it->first != name)
        return "";
    return it->second;
}

bool Request::getComplete() const {
    return _complete;
}

int Request::getContentLenght() const {
    return _content_length;
}

/* ----- OPERATORS ----- */

Request & Request::operator=(Request const &op) {
    _raw = op._raw;
    _req = op._req;
    _headers = op._headers;
    _body = op._body;
    _complete = op._complete;
    _content_length = op._content_length;
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

const   char* Request::ContentLengthException::what() const throw() {
    return "Content-Length too big";
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