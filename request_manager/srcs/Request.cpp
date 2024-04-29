/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 11:12:53 by nesdebie          #+#    #+#             */
/*   Updated: 2024/04/29 12:12:26 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

 /*_____CONSTRUCTORS_&_DESTRUCTOR_____*/

Request::Request() {
}

Request::Request(std::string &head, std::string &body): _body(body), _complete(true),  _expect(false), _content_length(0), _boundary_string(""){
    _raw = head + body;
    _parseRequest(head);
    if (_req.getMethod() == POST && getHeader("Content-Length").size()) {
        _content_length = atoi(getHeader("Content-Length").c_str());
        if (_content_length > CONTENT_LENGTH_MAX)
            throw ContentLengthException();
        if (_body.size() < _content_length)
            _complete = false;
    }
    if (_req.getMethod() == POST && !_headers.empty()) {
        map_strstr::iterator it = _headers.find("Expect");
        if (it->first == "Expect") {
            if (it->second == "100-continue")
                _expect = true;
        }
    }
}

Request::Request(Request const &copy) {
    *this = copy;
}

Request::~Request() {
}


/*_____PUBLIC_FUNCTIONS_____*/

void Request::catToBody(std::string & str) {
    _body += str;
    if (_body.size() == _content_length)
        _complete = true;
}


/*_____PRIVATE_FUNCTIONS_____*/

void Request::_parseRequest(std::string const & head) {
    std::istringstream  iss(head);
    std::string         line;
    int                 count = 0;

    while (std::getline(iss, line, '\n')) {
            if (line.size() == 0)
                continue ;
            if (count == 0) {
                vec_str arr = _vectorSplit(line, SPACE);
                std::string httpMethods[3] = {"DELETE", "GET", "POST"};
                int method;

                for (method = 0; method < 3 && httpMethods[method] != arr[0]; method++);
                _req =  RequestLine(method, arr[1], arr[2], arr[0]);
                count++;
                continue ;
            }
            size_t pos = line.find(':');
            if (pos == std::string::npos && _req.getMethod() != GET) {
                if (line.size() >= 2 && line[0] == '-' && line[1] == '-') {
                    _boundary_string = line;
                }
                continue ;
            }
            std::string headerName = line.substr(0, pos);
            std::string headerVal = line.substr(pos + 1);
            this->_headers.insert(std::make_pair(headerName, headerVal));
        }
}

vec_str Request::_vectorSplit(std::string str, char sep) {
    vec_str tokens;
    std::string token;
    std::istringstream iss(str);
    while (getline(iss, token, sep))
        if (!token.empty())
            tokens.push_back(token);
    for (int i = 0; i < 3; i++)
        std::cout << "============> ARR=[" << tokens[i] << "]\n";
    return tokens;
}

std::string Request::_strtrim(std::string &s) {
    s.erase(s.find_last_not_of(" \t\n\r") + 1);
    s.erase(0, s.find_first_not_of(" \t\n\r"));
    return s;
}


/*_____GETTERS_____*/

std::string Request::getRaw() const {
    return _raw;
}

RequestLine Request::getRequestLine() const {
    return _req;
}

std::string Request::getBody() const {
    return _body;
}

map_strstr Request::getHeaders() const {
    return _headers;
}

std::string Request::getHeader(std::string const &name) {
    if (this->_headers.size() == 0)
        return "";
    map_strstr::iterator it = this->_headers.find(name);
    if (it->first != name)
        return "";
    return it->second;
}

bool Request::getComplete() const {
    return _complete;
}

int Request::getContentLength() const {
    return _content_length;
}

int Request::getMethod() const {
    return _req.getMethod();
}

std::string Request::getHttpVersion() const {
    return _req.getHTTPVersion();
}

std::string Request::getPath() const {
    return _req.getPath();
}

std::string Request::getQuery() const {
    return _req.getQuery();
}

bool Request::getExpect() const {
    return _expect;
}

std::string Request::getBoundaryString() const {
    return _boundary_string;
}

/*_____SETTERS_____*/

void        Request::setHeaders(map_strstr const &headers) {
    if (!headers.empty())
        this->_headers = headers;
}

void        Request::setHeader(std::string &header, std::string &value) {
    map_strstr::iterator tmp = _headers.find(header);
    if (tmp->first == header) {
        _headers.erase(header);
        header = _strtrim(header);
        value = _strtrim(value);
        this->_headers.insert(std::make_pair(header, value));
    }
}

void        Request::setBody(std::string const &body) {
    if (!_body.empty())
        this->_body = body;
}


/*_____OPERATORS_____*/

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
    map_strstr headers = obj.getHeaders();
    map_strstr::iterator it = headers.begin();
    for (unsigned long i = 0; i < headers.size(); i++){
        o << it->first << ": " << it->second << std::endl;
        it++;
    }
    if (obj.getBody().size())
        o << std::endl << obj.getBody();
    return o;
}


/*_____EXCEPTIONS_____*/

const   char* Request::ContentLengthException::what() const throw() {
    return "Content-Length too big";
}
