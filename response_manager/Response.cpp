/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:58:55 by achansar          #+#    #+#             */
/*   Updated: 2024/03/26 10:17:16 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <filesystem>

// ============================================================================== CONSTRUCTORS

Response::Response(Server* server, int statusCode, const int method, const int socket) :
        _clientSocket(socket),
        _method(method),
        _statusCode(statusCode),
        _path("/"),
        _server(server) {

    return;
}

Response::~Response() {
    return;
}

// ============================================================================== MEMBER FUNCTIONS

// ==================================================================== FILE TRANSFER

int Response::sendFile(int socket) {

	std::cout << "\nIN SENDFILE :\n" << std::endl;

	std::ifstream	infile(_path, std::ios::binary | std::ios::in);
	if (!infile) {
		std::cout << "Le fichier s'ouvre pas." << std::endl;
		return 500;
	} else {
		infile.seekg(0, std::ios::end);
		std::streampos fileSize = infile.tellg();
		infile.seekg(0, std::ios::beg);

        std::string fileName = extractFileName();
        std::cout << "right before, filename is : " << fileName << std::endl;

		std::stringstream responseHeaders;
			responseHeaders << "HTTP/1.1 200 OK\r\n";
			responseHeaders << "Content-Type: " << getMimeType() << "\r\n";
			responseHeaders << "Content-Disposition: attachment; filename=\"" << fileName << "\"\r\n";
            responseHeaders << "Content-Length: " << fileSize << "\r\n\r\n";
			
		write(socket, responseHeaders.str().c_str(), responseHeaders.str().length());
		const std::streamsize bufferSize = 8192;
		char buffer[bufferSize];

        while (!infile.eof()) 
            {
                infile.read(buffer, sizeof(buffer));
                ssize_t result = write(socket, buffer, infile.gcount());
                if (result == -1) 
                {
                    std::cerr << "Error writing to socket." << std::endl;
                    break;
                }
            }

        infile.close();
	}
	return 200;
}

/*

TO DO (Arno)
	build receive() using read() from client socket, by chunks
	work on 300 reidrections
	do autoindexes
*/

int receiveFile(int socket, std::string uri, std::string raw) {
    
    (void)socket;
    (void)uri;
    (void)raw;
    
    /*
    use a std::getline() on headers first, to get content-type and content-length
    use the boundary string to get a size_t boundary_pos and place at begining of body
    use vector of char to store the body.
    in a while loop, read each chunk a leur tour
    sans oublier de checker si le retour est de 0 ET de 1
    */

	return 200;
}

int Response::fileTransfer(int socket, std::string uri, int method, std::string raw) {

    switch (method) {
        case GET:       return sendFile(socket);
        case POST:      return receiveFile(socket, uri, raw);
        case DELETE:    return 200;
        default:        return 200;
    }
}

// ==================================================================== POST METHOD


// ==================================================================== GET METHOD

std::string Response::getReason(int sc) {

    std::map<int, std::string> reasons;
    reasons.insert(std::make_pair(200, "OK"));
    reasons.insert(std::make_pair(201, "Created"));
    reasons.insert(std::make_pair(400, "Bad Request"));
    reasons.insert(std::make_pair(403, "Forbidden"));
    reasons.insert(std::make_pair(404, "Not Found"));
    reasons.insert(std::make_pair(500, "Internal Server Error"));

    std::map<int, std::string>::iterator it = reasons.find(sc);
    if (it == reasons.end()) {
        std::cout << "Status code not implemented !" << std::endl;
    }
    return it->second;
}

std::string Response::getHeaders(const int s) {//          which header is important ?
    std::string h;

    if (_method == GET) {
        h += "Content-Type: " + getMimeType() + "\r\n";
    }
    h += "Content-Length: " + std::to_string(s) + "\r\n";// virer tostirng
    return h;
}

std::string Response::getBody() {

	std::ifstream			myfile;
    std::string             line;
    std::string             body;

    if (_method == GET) {
        myfile.open(_path);
        if (myfile.fail()) {
            _statusCode = 500;
        }

        while (std::getline(myfile, line)) {
            body += line;
        }

        myfile.close();
    } else if (_method == POST) {
        body = "";
        // might be infos :
        // resource creation & confirmation message
    } else if (_method == DELETE) {
        body = "";
    }
    return body;
}

// ==================================================================== SWITCH


void      Response::buildResponse(Route *route, Request request, int socket) {

    // std::cout << "\nREQUEST ::\n" << request.getRaw() << std::endl;
    _extension = extractExtension(request.getPath());
    getFullPath(route, request.getPath());
    
    std::stringstream   ss;

    if ((!_extension.empty() && _extension.compare(".html")) || _method != GET) {
            _statusCode = fileTransfer(socket, request.getPath(), request.getMethod(), request.getRaw());
    }
    if (_statusCode == 200) {  
        _body = getBody();
        _headers = getHeaders(_body.length()) + "\n";
        ss << _statusCode;
        _statusLine = "HTTP/1.0 " + ss.str() + " " + getReason(_statusCode) + "\n";
    } else {
        buildErrorResponse();
    }
    _responseLine = _statusLine + _headers + _body;
    // std::cout << "\nRESPONSE :: \n" << _responseLine << std::endl;
    return;
}

// ============================================================================== GETTER & SETTER

std::string     Response::getResponse() {
    return _responseLine;
}

std::string     Response::getPath() {
    return _path;
}

int             Response::getStatusCode() {
    return _statusCode;
}

void            Response::setPath(std::string& str) {
    _path = str;
    return;
}

void            Response::setErrorPath(std::string& str) {
    _errorPath = str;
    return;
}

// ============================================================================== UTILS

std::string Response::extractExtension(std::string uri) {
    
    std::cout << "In extract extension, path : " << uri << std::endl;
    size_t extPos = uri.find_last_of(".");
    if (extPos != std::string::npos) {
        std::string extension = uri.substr(extPos, std::string::npos);
        return extension;
        std::cout << "EXTENSION IS : " << extension << std::endl;    
    } else {
        std::cerr << "Couldn't extract extension.\n";
        return "";
    }
}

std::string Response::extractFileName() {
    
    size_t extPos = _path.find_last_of("/");
    if (extPos != std::string::npos) {
        std::string fileName = _path.substr(extPos + 1, std::string::npos);
        std::cout << "Filename is : " << fileName << " from a path of : " << _path << std::endl;
        return fileName;    
    } else {
        std::cerr << "Couldn't extract file name.\n";
        return NULL;
    }
}

std::string     Response::getMimeType() {

    if (!_extension.empty()) {
        return _server->getMimeType(_extension);
    }
    return _server->getMimeType("default");
}



void	Response::getFullPath(Route *route, std::string uri) {

	if (route) {
		std::string root = route->getRoot() + "/";
		std::string index = route->getIndex().front();//         only gets first index. it's temporary
		_path = root + index;
	} else {
		std::ifstream			myfile;

        if (_extension.compare(".html")) {
            _path = "./download" + uri;
        } else {
            _path = "./docs" + uri;
        }
        // std::cout << "Because URI : " << uri << ", _extension : " << _extension << "and _path became : " << _path << std::endl;
		myfile.open(_path);
		if (myfile.fail()) {
			_statusCode = 404;
			_path = "/";
		} else {
			myfile.close();
		}
	}
	return;
}
