/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 21:08:23 by nesdebie          #+#    #+#             */
/*   Updated: 2024/04/19 00:01:44 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cgi.hpp"

Cgi::Cgi() {
}

Cgi::Cgi(Request const &request, Route const &route) : _request(request), _route(route), _envp(NULL), _exitCode(500) {
	if (!_route.getCgi())
		throw NotCgiException();
    if (!_request.getQuery().size())
	    _fileToExec = "." + _request.getPath(); // fichier a executer
    else {
        _fileToExec = _request.getPath() + "?" + _request.getQuery();
    }
	_executablePath = _route.getPath(); // localisation de l'executable
}

Cgi::~Cgi() {
    if (_envp)
        _freeArray(_envp, -1);
}

Cgi::Cgi(Cgi const &copy) {
    *this = copy;
}

/* ----- FUNCTIONS ----- */

std::string Cgi::executeCgi() {
    std::string ret = "";

	if (access(_fileToExec.c_str(), F_OK) != 0)
		throw FileNotFoundException();

    std::string extension = _getFileExtension(_fileToExec, '.');
    if (!extension.size())
        throw UnsupportedExtensionException();

    if (_route.getExtension() != extension || (extension != ".py" && extension != ".pl"))
        throw UnsupportedExtensionException();

    int pipefdin[2];
    int pipefdout[2];
    if (pipe(pipefdin) == -1) {
        throw PipeException();
    }
    if (pipe(pipefdout) == -1) {
        close (pipefdin[0]);
        close (pipefdin[1]);
        throw PipeException();
    }
    if (!_request.getHeaders().empty())
            _envp = _createEnv();
    int fdin = dup(STDIN_FILENO);
    int fdout = dup(STDOUT_FILENO);
    
    pid_t pid = fork();
    if (pid == -1) {
        close(pipefdin[0]);
        close(pipefdin[1]);
        close(pipefdout[0]);
        close(pipefdout[1]);
        close(fdin);
        close(fdout);
        _freeArray(_envp, -1);
        _envp = NULL;
        throw ForkException();
    }

    // CHILD
    if (pid == 0) {
        close(pipefdin[1]);
        close(pipefdout[0]);
        dup2(pipefdin[0], STDIN_FILENO);
        dup2(pipefdin[1], STDOUT_FILENO);
        const char *exec = _executablePath.c_str();
        std::string exe = _getFileExtension(_executablePath, '/');
        char const *args[3] = {exe.c_str(), _fileToExec.c_str(), NULL};
        execve(exec, const_cast<char *const *>(args), _envp);            
        std::cerr << "Error executing CGI." << std::endl;
        _freeArray(_envp, -1);
        std::exit(EXIT_FAILURE);
    } else { //PARENT

        pid_t timeOut = fork();
        if (timeOut == -1) {
            close(pipefdin[0]);
            close(pipefdin[1]);
            _freeArray(_envp, -1);
            _envp = NULL;
            throw ForkException();
        }
        if (!timeOut) {
            struct timeval tv;
            tv.tv_sec = CGI_TIMEOUT;
            tv.tv_usec = 0;
            select(0, NULL, NULL, NULL, &tv);
            kill(pid, SIGTERM);
            std::exit(2);
        } else {
            close(pipefdin[0]);
            close(pipefdout[1]);
            char buffer[1023];
            dup2(pipefdin[1], STDIN_FILENO);
            dup2(pipefdout[0], STDOUT_FILENO);
            int post;
            if (_request.getMethod() == POST)
                {
                        std::stringstream ss;
                        ss << _request.getQuery().size();
                        _envp["CONTENT_LENGTH"] = std::string(ss.str());
                        post = write(pipefdin[1], _request.getQuery().c_str(), _request.getQuery().size());
                }
            int status;
            int bytesRead;
            waitpid(pid, &status, 0);
            kill(timeOut, SIGTERM);
            while (status && ((bytesRead = read(pipefdout[0], buffer, 1023)) > 0)) {
                buffer[bytesRead] = 0;
                ret += std::string(buffer, bytesRead);
            }
            close(pipefdin[1]);
            close(pipefdout[0]);
            dup2(fdin, STDIN_FILENO);
            dup2(fdout, STDOUT_FILENO);
            if (status || (_request.getMethod() == POST && post != _request.getQuery().size())) {
                std::cerr << "Child process exited with an error." << std::endl;
            }
        }
    }
    _freeArray(_envp, -1);
    close(fdin);
    close(fdout);
    _exitCode = 200;
    return ret;
}

char **Cgi::_createEnv() {
	map_strstr mapEnv;
	const char *methods[] = {"DELETE", "GET", "POST", NULL};
	mapEnv.insert(std::make_pair("REQUEST_METHOD", methods[_request.getMethod()]));
    std::string pwd(_executablePath);
    pwd.erase(pwd.find_last_of('/'), pwd.size());
	mapEnv.insert(std::make_pair("PWD", pwd));
	if (_request.getMethod() != POST)
		mapEnv.insert(std::make_pair("QUERY_STRING", _request.getQuery()));
	mapEnv.insert(std::make_pair("CONTENT_TYPE", _request.getHeader("Content-Type")));
	std::stringstream content_length;
	content_length << _request.getContentLength();
	mapEnv.insert(std::make_pair("CONTENT_LENGTH", content_length.str()));
	mapEnv.insert(std::make_pair("SERVER_NAME", _route.getServer()->getServerNames().front()));
    std::stringstream port;
	port << _route.getServer()->getPort();
	mapEnv.insert(std::make_pair("SERVER_PORT", port.str()));
	mapEnv.insert(std::make_pair("SCRIPT_NAME", _request.getPath()));
	mapEnv.insert(std::make_pair("PATH_INFO", _request.getPath()));
	mapEnv.insert(std::make_pair("SERVER_PROTOCOL", _request.getHttpVersion()));

    char **ret;
    try {
        ret = new char *[mapEnv.size() + 1];
    } catch (std::bad_alloc &ba) {
        return NULL;
    }
	int i = 0;
    for (map_strstr::iterator it = mapEnv.begin(); it != mapEnv.end(); it++)
    {
        std::string tmp = it->first + "=" + it->second;
        ret[i] = new char[tmp.size() + 1];
		if (!ret[i]) {
			_freeArray(ret, i);
			return NULL;
		}
        strcpy(ret[i], tmp.c_str());
        i++;
    }
    ret[i] = NULL;
	return ret;
}

void    Cgi::_freeArray(char **arr, int flag) {
	if (flag == -1)
        for (size_t i = 0; arr[i]; i++)
		    delete[] arr[i];
    else {
        for (int i = 0; i < flag; i++)
            delete[] arr[i];
    }
	delete[] arr;	  
}

std::string Cgi::_getFileExtension(const std::string& _fileToExec, char sep) {
    size_t dotPos = _fileToExec.find_last_of(sep);
    if (dotPos != std::string::npos) {
        if (sep == '/' && (dotPos + 1) != std::string::npos)
            dotPos++;
        return _fileToExec.c_str() + dotPos;
    }
    return "";
}

/* ----- GETTERS ----- */

int Cgi::getExitCode() const {
	return _exitCode;
}

Request Cgi::getRequest() const {
	return _request;
}

Route Cgi::getRoute() const {
	return _route;
}

std::string Cgi::getFileToExec() const {
	return _fileToExec;
}

std::string Cgi::getExecutablePath() const {
	return _executablePath;
}

char** Cgi::getEnvp() const {
	return _envp;
}


/* ----- OPERATORS ----- */

Cgi & Cgi::operator=(Cgi const &op) {
    _request = op._request;
    _route = op._route;
    _fileToExec = op._fileToExec;
    _executablePath = op._executablePath;
    _envp = op._envp;
    return *this;
}

std::ostream & operator<<(std::ostream &o, Cgi const &obj) {
	o << "Request" << std::endl << obj.getRequest() << std::endl;
	o << "Executable:" << std::endl << "|" << obj.getExecutablePath() << std::endl;
	o << "File to execute" << std::endl << "|" << obj.getFileToExec() << std::endl;
    if (obj.getEnvp())
	{
		char **tmp = obj.getEnvp();
		for(size_t i = 0; tmp[i]; i++)
			o << tmp[i] << std::endl;
	}
    return o;
}


/* ----- EXCEPTIONS ----- */

const   char* Cgi::PipeException::what() const throw() {
    return "CgiException: pipe() failed.";
}

const   char* Cgi::ForkException::what() const throw() {
    return "CgiException: fork() failed.";
}

const   char* Cgi::NotCgiException::what() const throw() {
    return "CgiException: not a CGI";
}

const   char* Cgi::FileNotFoundException::what() const throw() {
    return "CgiException: file not found";
}

const   char* Cgi::UnsupportedExtensionException::what() const throw() {
    return "CgiException: unsupported extention";
}

