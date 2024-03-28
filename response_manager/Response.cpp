/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:58:55 by achansar          #+#    #+#             */
/*   Updated: 2024/03/28 17:25:45 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <filesystem>

// ============================================================================== CONSTRUCTORS

Response::Response(Server* server, int statusCode, Request* request, const int socket) :
        _clientSocket(socket),
        _method(request->getMethod()),
        _statusCode(statusCode),
        _path("/"),
        _server(server),
        _request(request) {

    return;
}

Response::~Response() {
    return;
}

// ============================================================================== MEMBER FUNCTIONS

// ==================================================================== FILE TRANSFER

int Response::sendFile() {

	std::ifstream	infile(_path, std::ios::binary | std::ios::in);
	if (!infile) {
		std::cout << "Le fichier ne s'ouvre pas." << std::endl;
		return 500;
	} else {
		infile.seekg(0, std::ios::end);
		std::streampos fileSize = infile.tellg();
		infile.seekg(0, std::ios::beg);

        std::string fileName = extractFileName();

		std::stringstream responseHeaders;
			responseHeaders << "HTTP/1.1 200 OK\r\n";
			responseHeaders << "Content-Type: " << getMimeType() << "\r\n";
			responseHeaders << "Content-Disposition: attachment; filename=\"" << fileName << "\"\r\n";
            responseHeaders << "Content-Length: " << fileSize << "\r\n\r\n";
			
		write(_clientSocket, responseHeaders.str().c_str(), responseHeaders.str().length());
		const std::streamsize bufferSize = 8192;
		char buffer[bufferSize];

        while (!infile.eof()) 
            {
                infile.read(buffer, sizeof(buffer));
                ssize_t result = write(_clientSocket, buffer, infile.gcount());
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
	work on 300 reidrections
    build deleteFiles
	do autoindexes
*/

int Response::receiveFile() {

    std::cout << "IN RECEIVE FILE\n";
    
    std::stringstream rawRequest(_request->getRaw());
    std::string line;
    std::string fileName;
    size_t Pos = 0;

    while (std::getline(rawRequest, line) && !line.empty()) {
        if (line.find("Content-Disposition") != std::string::npos) {
            Pos = line.find_last_of("=");
            if (Pos != std::string::npos) {
                size_t endPos = line.find_last_of('"');
                fileName = line.substr(Pos + 2, endPos - Pos - 2);
            }
        }
    }

    std::string fileBody = extractFileBody(_request->getRaw());
    // if (fileBody.empty()) {
    //     return 400;
    // }
    std::string destination = "." + _request->getPath() + "/" + fileName;
    // std::cout << "dest : " << destination << std::endl;
    // std::ofstream outputFile(destination);
    // if (!outputFile) {
    //     std::cerr << "Error creating file" << std::endl;
    //     return 500;
    // }
    // outputFile << fileBody;
    // outputFile.close();
                
    std::ofstream targetFile(destination, std::ios::binary);
    if (!targetFile) {
        std::cerr << "Error creating the file.\n";
    }
    targetFile.write(fileBody.c_str(), fileBody.size());
    targetFile.close();
	return 200;
}

int Response::deleteFile() {
    
    
    
    return 200;
}

int Response::fileTransfer() {//           FAUT-IL UTILISER SELECT/POLL ?

    std::cout << "In file transfer, method is " << _method << std::endl;
    switch (_method) {
        case GET:       return sendFile();
        case POST:      return receiveFile();
        case DELETE:    return 200;
        default:        return 200;
    }
}

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


void      Response::buildResponse(Route *route) {

    _extension = extractExtension(_request->getPath());
    getFullPath(route, _request->getPath());
    
    std::stringstream   ss;

    std::cout   << "\n\nREQUEST IN BUILD:\n------------------------------------------------------------------------------------------------------\n"
                << _request->getRaw()
                << "\n------------------------------------------------------------------------------------------------------\n\n" << std::endl;
    // std::cout << "Before sending file, extension is : " << _extension << std::endl;
    if ((!_extension.empty() && _extension.compare(".html")) || _method != GET) {
            _statusCode = fileTransfer();
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
        return fileName;    
    } else {
        std::cerr << "Couldn't extract file name.\n";
        return NULL;
    }
}

std::string Response::extractFileBody(std::string request) {
    
    size_t boundaryPos = request.find("Content-Type");
    if (boundaryPos == std::string::npos) {
        return "";
    }
    size_t Pos = request.find("\n", boundaryPos) + 3;
    if (Pos == std::string::npos) {
        return "";
    }
    size_t endPos = request.find("\n------WebKitFormBoundary", Pos) - 1;
    if (endPos == std::string::npos) {
        return "";
    }
    std::string fileBody = request.substr(Pos, endPos - Pos);
    return fileBody;
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
