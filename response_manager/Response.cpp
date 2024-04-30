/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:58:55 by achansar          #+#    #+#             */
/*   Updated: 2024/04/30 14:10:10 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

// ============================================================================== CONSTRUCTORS

Response::Response(Server* server, int statusCode, Request* request, const int socket, std::string uri) :
        _bytesSend(0),
        _cgi(false),
        _clientSocket(socket),
        _method(request->getMethod()),
        _statusCode(statusCode),
        _path(uri),
        _server(server),
        _request(request) {

    return;
}

Response::Response(const Response& src) {
    *this = src;
    return;
}

Response& Response::operator=(const Response& src) {
        _clientSocket = src._clientSocket;
        _bytesSend = src._bytesSend;
        _method = src._method;
        _statusCode = src._statusCode;
        _path = src._path;
        _errorPath = src._errorPath;
        _responseLine = src._responseLine;
        _statusLine = src._statusLine;
        _extension = src._extension;
        _headers = src._headers;
        _body = src._body;
        _server = src._server;
        _request = src._request;
        return *this;
}

Response::~Response() {
    return;
}

// ============================================================================== MEMBER FUNCTIONS

// ==================================================================== DELETE

int Response::deleteFile() {

    if (std::remove(_path.c_str()) != 0) {
        return 404;
    }
    return 204;
}

// ==================================================================== POST

void Response::sendFile() {

	std::ifstream	infile(_path.c_str(), std::ios::binary | std::ios::in);
	if (!infile) {
		std::cerr << "Error opening local file." << std::endl;
		_statusCode = 404;
        return;
	} else {
		char buffer[8192];
        while (!infile.eof())
            {
                bzero(buffer, sizeof(buffer));
                infile.read(buffer, sizeof(buffer));
                _body.append(buffer, infile.gcount());
            }
        infile.close();
	}
	_statusCode = 200;
    return;
}

int Response::receiveFile() {

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
    if (fileBody.empty()) {
        return 400;
    }
    std::string destination = "";
    if (!_request->getPath().compare(0, 9, "/cgi-bin/")) {
        destination = "./upload/" + fileName;
    } else {
        destination = "." + _request->getPath() + "/" + fileName;
    }
    if (access(destination.c_str(), F_OK) != -1) {
        std::cerr << "File already exists." << std::endl;
        return 409;
    }

    std::ofstream targetFile(destination.c_str(), std::ios::binary);
    if (!targetFile) {
        std::cerr << "Error creating the file: "<< destination<< ".\n";
    }
    targetFile.write(fileBody.c_str(), fileBody.size());
    targetFile.close();
	return 201;
}

// ==================================================================== GET METHOD

std::string    convertEncoding(std::string s) {

    std::string str = s;
    size_t pos = str.find('%');

    while (pos != std::string::npos) {
        std::string sub = str.substr(pos + 1, 2);
        int deci = strtol(sub.c_str(), NULL, 16);
        char special = deci;
        str.replace(pos, 3, 1, special);
        pos = str.find('%', pos);
    }

    pos = str.find('+');
    while (pos != std::string::npos) {
        str[pos] = ' ';
        pos = str.find('+', pos + 1);
    }
    return str;
}

int Response::handleForm() {

    std::istringstream formBody(_request->getBody());
    std::map<std::string, std::string> formInfos;
    std::vector<std::string> formElements;

    for (std::string ele; std::getline(formBody, ele, '&'); ) {
        formElements.push_back(ele);
    }

    for (std::vector<std::string>::iterator it = formElements.begin(); it != formElements.end(); it++) {
        std::string str = *it;
        size_t pos = str.find('=');
        std::string key = convertEncoding(str.substr(0, pos));
        std::string value = convertEncoding(str.substr(pos + 1));
        formInfos[key] = value;
    }

    return 201;
}

int Response::handlePostRequest() {

    if (_path == "/submitForm") {
        return handleForm();
    } else {
        return receiveFile();
    }
}


int Response::fileTransfer() {

    switch (_method) {
        case POST:      return handlePostRequest();
        case DELETE:    return deleteFile();
        case UNVALID:   return 405;
        default:        return 200;
    }
}

std::string Response::getReason(int sc) {

    std::map<int, std::string> reasons;
    reasons.insert(std::make_pair(200, "OK"));
    reasons.insert(std::make_pair(201, "Created"));
    reasons.insert(std::make_pair(204, "No Content"));
    reasons.insert(std::make_pair(400, "Bad Request"));
    reasons.insert(std::make_pair(403, "Forbidden"));
    reasons.insert(std::make_pair(404, "Not Found"));
    reasons.insert(std::make_pair(405, "Method Not Allowed"));
    reasons.insert(std::make_pair(409, "Conflict"));
    reasons.insert(std::make_pair(500, "Internal Server Error"));
    reasons.insert(std::make_pair(504, "Gateway Timeout"));
    reasons.insert(std::make_pair(501, "Not Implemented"));


    std::map<int, std::string>::iterator it = reasons.find(sc);
    if (it == reasons.end()) {
        std::map<int, std::string>::reverse_iterator itrev = reasons.rbegin();
        return itrev->second;
    }
    return it->second;
}

std::string Response::getHeaders(const int s) {

    std::string h;
    std::ostringstream intss;
    intss << s;

    if (_method == GET) {
        h += "Content-Type: " + getMimeType() + "\r\n";
    }
    h += "Content-Length: " + intss.str() + "\r\n";

    if (!_extension.empty() && _cgi == false) {
        std::string fileName = extractFileName();
        if (_path.find("/upload/") != std::string::npos)
	        h += "Content-Disposition: attachment; filename=\"" + fileName + "\"\r\n";
        else if (_extension == ".css") {
	        h += "Content-Disposition: inline; filename=\"" + fileName + "\"\r\n";
        } else if (_extension.compare(".html") && _extension != ".ico") {
	        h += "Content-Disposition: attachment; filename=\"" + fileName + "\"\r\n";
        }
    }
    return h;
}

void Response::getBody(bool autoindex, Route *route) {

	std::ifstream			myfile;
    std::string             line;

	if (route) {
		if ((!route->getExtension().empty()) || _request->getMethod() == POST || (_request->getMethod() == GET && _request->getPath().compare("/"))) {
				// std::cout << "[CGI] START" << std::endl;
				try {
					Cgi cgi(*_request, *route);
				    _body = cgi.executeCgi();
					_statusCode = cgi.getExitCode();
                    _cgi = true;
					return ;
				}
                catch (Cgi::NotCgiException const &e) {
                    std::cerr << e.what() << std::endl;
                }
				catch (Cgi::PipeException const &e) {
					std::cerr << e.what() << std::endl;
                    _body = "";
                    _statusCode = 500;
                    _cgi = true;
                    return ;
				}
				catch (Cgi::ForkException const &e) {
					std::cerr << e.what() << std::endl;
                    _body = "";
                    _statusCode = 500;
                    _cgi = true;
                    return ;
				}
				catch (std::bad_alloc const &e) {
					std::cerr << e.what() << std::endl;
                    _body = "";
                    _statusCode = 500;
                    _cgi = true;
                    return ;
				}
		}
    }

    if (_method == GET) {
        if (isDirectory(_path)) {
            if (autoindex) {
                _statusCode = generateAutoindex();
            } else {
                _statusCode = 403;
            }
        } else {
            sendFile();
        }
    } else if (_method == POST || _method == DELETE) {
        _body = "";
    }
}

// ==================================================================== BUILD RESPONSE

void	Response::getFullPath(Route *route) {

    _path = "." + _path;
    // std::cout << "START OF GETFULLPATH , URI IS [" << _path << "]" << std::endl;
    if (!route->getRedirection().empty()) {
        _path = route->getRedirection();
        _statusCode = 301;
    }
    else if ((_path.back() != '/' || _path == "./") && isDirectory(_path) && _method == GET) {
        if (_path.back() != '/')
            _path += "/";
        _path += route->getIndex().front();
    }
    // std::cout << "END OF GETFULLPATH , PATH IS [" << _path << "]" << std::endl;
	return;
}

void      Response::buildResponse(Route *route) {

    bool autodindex = false;

    if (!route)
        _statusCode = 404;
    if (route) {
        _statusCode = checkAllow(route);
        autodindex = route->getAutoindex();
        _extension = extractExtension(_request->getPath());
        getFullPath(route);
    }

    std::stringstream   ss;

    std::cout   << "\n\nREQUEST IN BUILD:\n------------------------------------------------------------------------------------------------------\n"
                << _request->getRaw()
                << "\n------------------------------------------------------------------------------------------------------\n\n" << std::endl;

    if (((!_extension.empty() && _extension.compare(".html")) || _method != GET)  && _statusCode == 200 && !route->getCgi()) {
            _statusCode = fileTransfer();
    }

    if (_statusCode == 200 || _statusCode == 201 || _statusCode == 204) {
        getBody(autodindex, route);
        _headers = getHeaders(_body.length()) + "\n";
        ss << _statusCode;
        _statusLine = "HTTP/1.1 " + ss.str() + " " + getReason(_statusCode) + "\n";
    }
    if (_statusCode != 200 && _statusCode != 204 && _statusCode != 201) {
        buildErrorResponse();
    }
    _responseLine = _statusLine + _headers + _body;

    // std::cout << "\nRESPONSE :: \n" << _responseLine << std::endl << "And SOCKET IS : " << _clientSocket << std::endl;
    std::cout   << "\nRESPONSE HEAD:\n------------------------------------------------------------------------------------------------------\n"
                << _statusLine << _headers
                << "------------------------------------------------------------------------------------------------------\n\n" << std::endl;
    return;
}

// ============================================================================== GETTER & SETTER

unsigned long   Response::getBytesSend() const{
    return _bytesSend;
}

std::string   Response::getNextChunk() const{
    return (this->_responseLine.substr(_bytesSend, SENDCHUNKSIZE));
}

std::string     Response::getResponse() const {
    return _responseLine;
}

std::string     Response::getPath() const {
    return _path;
}

int             Response::getStatusCode() const {
    return _statusCode;
}

int Response::getClientSocket() const {
    return _clientSocket;
}

void            Response::addToBytesSend(unsigned long bytes_to_add)
{
    _bytesSend += bytes_to_add;
}

void            Response::setPath(std::string& str) {
    _path = str;
    return;
}

void            Response::setErrorPath(std::string& str) {
    _errorPath = str;
    return;
}

void            Response::setBody(std::string& str) {
    _body = str;
    return;
}

void            Response::setHeaders(std::string& str) {
    _headers = str;
    return;
}

// ============================================================================== UTILS

int Response::checkAllow(Route *route) {
    switch (_method) {
        case GET: return (route->getGet() ? 200 : 405);
        case POST: return (route->getPost() ? 200: 405);
        case DELETE: return (route->getDelete() ? 200 : 405);
        default: return 200;
    }
}

std::string Response::extractExtension(std::string uri) {

    size_t extPos = uri.find_last_of(".");
    if (extPos != std::string::npos) {
        std::string extension = uri.substr(extPos, std::string::npos);
        return extension;
    } else {
        // std::cerr << "Couldn't extract extension.\n";
        ;
    }
    return "";
}

std::string Response::extractFileName() {

    size_t extPos = _path.find_last_of("/");
    if (extPos != std::string::npos) {
        std::string fileName = _path.substr(extPos + 1, std::string::npos);
        return fileName;
    } else {
        // std::cerr << "Couldn't extract file name.\n";
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
        if (_extension == ".py" || _extension == ".pl")
            return _server->getMimeType("default");
        else
            return _server->getMimeType(_extension);
    }
    return _server->getMimeType("default");
}

bool Response::isDirectory(std::string path) {
    struct stat fileStat;
    if (path[_path.size() - 1] != '/')
                path += "/";
    if (stat(path.c_str(), &fileStat) == 0) {
        return S_ISDIR(fileStat.st_mode);
    }
    return false;
}

int Response::generateAutoindex() {

    std::stringstream response;

    if (_path[_path.size() - 1] != '/')
        _path += "/";

    DIR* dir = opendir(_path.c_str());
    if (!dir) {
        std::cerr << "Error opening directory.\n";
        return 500;
    }

    response << "<html><head><title>Autoindex</title></head><body>";
    response << "<h1>Autoindex</h1>";
    response << "<ul>";

    struct dirent* entry;
    while ((entry = readdir(dir))) {
        std::string name = entry->d_name;
        if (name != "." && name != "..") {
            response << "<li>" << name << "</li>";
        }
    }

    closedir(dir);
    response << "</ul></body></html>";
    _body = response.str();
    return 200;
}
