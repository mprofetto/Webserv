/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:58:55 by achansar          #+#    #+#             */
/*   Updated: 2024/03/24 16:56:47 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <filesystem>

// ============================================================================== CONSTRUCTORS

Response::Response(Server* server, int statusCode, const int method) :
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

int sendFile(int socket, std::string uri) {
	
	/*
	write a response with headers and all 
	then write in socket 
	*/
	
	std::cout << "\nIN SENDFILE :\n" << "\nuri : " << uri << std::endl;


	std::string newuri = "." + uri;
	std::ifstream	infile(newuri, std::ios::binary | std::ios::in);
	if (!infile) {
		std::cout << "Le fichier s'ouvre pas." << std::endl;
		return 500;
	} else {
		// infile.seekg(0, std::ios::end);
		// std::streampos fileSize = infile.tellg();
		// infile.seekg(0, std::ios::beg);

		// std::ostringstream responseHeaders;
		// 	responseHeaders << "HTTP/1.1 200 OK\r\n";
		// 	responseHeaders << "Content-Type: " << getMimeType(uri) << "\r\n";
		// 	responseHeaders << "Content-Disposition: attachment; filename=\"" << /*decodedFilename*/ "hello.txt" << "\"\r\n";

		// responseHeaders << "Content-Length: " << fileSize << "\r\n\r\n";
			
		// 	write(socket, responseHeaders.str().c_str(), responseHeaders.str().length());
		// const std::streamsize bufferSize = 8192; // Adjust the buffer size as needed
		// 	char buffer[bufferSize];

		// 	while (!infile.eof()) 
		// 		{
		// 			infile.read(buffer, sizeof(buffer));
		// 			ssize_t result = write(socket, buffer, infile.gcount());
		// 			if (result == -1) 
		// 			{
		// 				std::cerr << "Error writing to socket." << std::endl;
		// 				break;
		// 			}
		// 		}

		// 	infile.close();
		char buffer[1024];//                     SEEMS TO WORK BETTER WITH WRITE ON SOCKET

		std::string body = "";
		while (!infile.eof()) {
			infile.getline(buffer, 1024);
			body += buffer;
		}

		std::string response = "HTTP/1.0 200 OK\nContent-Type: text/html\nContent-Disposition: attachment; filename=\""/*decodedFilename*/"hello.txt\n"/* + body*/;
		int bytesSent = send(socket, response.c_str(), response.length(), 0);
		if (bytesSent == -1) {
			std::cerr << "\n\nError sending the file\n\n" << std::endl;
			return 500;
		}
		bytesSent = send(socket, body.c_str(), body.length(), 0);
				if (bytesSent == -1) {
			std::cerr << "\n\nError sending the file\n\n" << std::endl;
			return 500;
		} else {
			std::cout << "Body sent" << std::endl;
		}
		infile.close();
	}
	return 200;
}

// int receiveFile(int socket, ) {

// 	return 200;
// }

int Response::fileTransfer(int socket, std::string uri, int method) {

	if (method == POST) {
		return 200;
	} else if (method == GET) {
		std::cout << "\n\nCATCHING GET METHOD\n\n";
		return sendFile(socket, uri);
	} else {
		return 200;
	}
}

// ==================================================================== POST METHOD

void Response::buildPostResponse(Request request) {
    (void)request;
    return;
}

// ==================================================================== GET METHOD

std::string Response::getReason(int sc) {
//     the map better be part of the server object

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

std::string     Response::getMimeType() {

    std::map<std::string, std::string> MIMEtypes;
    MIMEtypes.insert(std::make_pair(".html", "text/html"));
    MIMEtypes.insert(std::make_pair(".txt", "text/plain"));
    MIMEtypes.insert(std::make_pair(".jpg", "image/jpeg"));
    MIMEtypes.insert(std::make_pair(".jpeg", "image/jpeg"));
    MIMEtypes.insert(std::make_pair(".png", "image/png"));
    MIMEtypes.insert(std::make_pair(".pdf", "application/pdf"));
    MIMEtypes.insert(std::make_pair("default", "text/html"));

    size_t extPos = _path.find_last_of(".");
    if (extPos != std::string::npos) {
        std::string extension = _path.substr(extPos, std::string::npos);

        std::map<std::string, std::string>:: iterator it = MIMEtypes.find(extension);
        if (it != MIMEtypes.end()) {
            return it->second;
        }
    }
    return MIMEtypes["default"];
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

// finish get response + redirections 3XX (cf "Should I include a slash (/) at the end of my URLs?")
// start delete response

// ==================================================================== SWITCH


void      Response::buildResponse(Request request) {

    std::cout << "\nREQUEST ::\n" << request.getRaw() << std::endl;
    std::cout << "Request line parsed : " << request.getRequestLine().getPath() << std::endl; 

    std::stringstream   ss;
    (void)request;


// int Response::fileTransfer(int socket, std::string uri, int method)

    if (_statusCode == 200) {        
        // switch (request.getRequestLine().getMethod()) {
        //     case 1:
        //         buildGetResponse(request);
        //         break;
        //     case 2:
        //         buildPostResponse(request);
        //         break;
        //     case 0:// DELETE
        //         break;
        //     default:
        //         break;//    ?
        // }
        _body = getBody();
        _headers = getHeaders(_body.length()) + "\n";

        ss << _statusCode;
        _statusLine = "HTTP/1.0 " + ss.str() + " " + getReason(_statusCode) + "\n";
    } else {
        buildErrorResponse();
    }
    _responseLine = _statusLine + _headers + _body;
    std::cout << "\nRESPONSE :: \n" << _responseLine << std::endl;
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

void	Response::getFullPath(Route *route, std::string uri) {

	if (route) {
		std::string root = route->getRoot() + "/";
		std::string index = route->getIndex().front();//         only gets first index. it's temporary
		_path = root + index;
	} else {
		std::ifstream			myfile;

		_path = "docs" + uri;
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
