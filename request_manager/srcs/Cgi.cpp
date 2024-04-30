/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/04/30 12:57:54 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/Cgi.hpp"

/*______CONSTRUCTORS_&_DESTRUCTORS_____*/

Cgi::Cgi() {
}

Cgi::Cgi(Request const &request, Route const &route) : _request(request), _route(route), _envp(NULL), _exitCode(500) {
	if (!_route.getCgi())
		throw NotCgiException();
    _fileToExec = _request.getPath();
    size_t pos = _fileToExec.find_last_of('/');
    if (pos != std::string::npos)
        _fileToExec = std::string(_fileToExec.begin() + pos, _fileToExec.end());

    if (!_request.getQuery().size())
	    _fileToExec = "." + _fileToExec;
    else {
        _fileToExec = "." + _fileToExec + "?" + _request.getQuery();
    }
	_executablePath = _route.getPath();
    if (!_request.getHeaders().empty())
        _envp = _createEnv();
}

Cgi::Cgi(Cgi const &copy) {
    *this = copy;
}

Cgi::~Cgi() {
    if (_envp)
        _freeArray(_envp, -1);
}


/*_____PUBLIC_FUNCTIONS_____*/

std::string Cgi::executeCgi() {
    std::string ret = "";

    int pipefdout[2];
    if (pipe(pipefdout) == -1) {
        throw PipeException();
    }

    int pipefdin[2];
    if (pipe(pipefdin) == -1) {
        close(pipefdout[0]);
        close(pipefdout[1]);
        throw PipeException();
    }

    pid_t pid_execve = fork();
    if (pid_execve == -1) {
        close(pipefdout[0]);
        close(pipefdout[1]);
        close(pipefdin[0]);
        close(pipefdin[1]);
        throw ForkException();
    }
    if (pid_execve == 0) {
		close(pipefdin[1]);
		close(pipefdout[0]);
		dup2(pipefdin[0], STDIN_FILENO);
		dup2(pipefdout[1], STDOUT_FILENO);
        const char *exec = _executablePath.c_str();
        std::string exe = _getFileExtension(_executablePath, '/');
        char const *args[3] = {exe.c_str(), _fileToExec.c_str(), NULL};
        std::string path = "." + _request.getPath();
        path.erase(path.find_last_of('/'), path.size());
		if (!chdir(path.c_str()))
            execve(exec, const_cast<char *const *>(args), _envp);
        // std::cerr << "Error executing CGI :" << _fileToExec << std::endl;
        std::exit(500);
    } else if (pid_execve > 0) {
        pid_t pid_timeout = fork();
        if (pid_timeout == -1) {
            close(pipefdout[0]);
            close(pipefdout[1]);
            close(pipefdin[0]);
            close(pipefdin[1]);
            _envp = NULL;
            throw ForkException();
        }
        if (pid_timeout == 0) {
            struct timeval tv;
            tv.tv_sec = CGI_TIMEOUT;
            tv.tv_usec = 0;
            select(0, NULL, NULL, NULL, &tv);
            kill(pid_execve, SIGTERM);
            std::exit(504);
        } else if (pid_timeout > 0) {
            close(pipefdout[1]);

            size_t post;
            if (_request.getMethod() == POST) {
                post = write(pipefdin[1], _request.getBody().c_str(), _request.getBody().size());
            }

            int status;
            waitpid(pid_execve, &status, 0);
            if (WIFEXITED(status)) {
                kill(pid_timeout, SIGTERM);
                int exitStatus = WEXITSTATUS(status);
                if (exitStatus == 0) {
                    
                    char buffer[254];
                    ssize_t bytesRead;
                    while ((bytesRead = read(pipefdout[0], buffer, 253)) > 0) {
                        buffer[bytesRead] = 0;
                        ret.append(buffer, bytesRead);
                        if (bytesRead < 253)
                            break ;
                    }
                    close(pipefdin[1]);
                    close(pipefdout[0]);

                    _exitCode = 200;
                    if (_request.getMethod() == POST)
                        _exitCode = 201;
                } else {
                    _exitCode = 500;
                }
            } else {
                _exitCode = 504;
                // if (_request.getMethod() == POST && post != _request.getQuery().size()) {
                    // std::cerr << "Child process exited with an error." << std::endl;
                // }
            }
            waitpid(pid_timeout, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 504) {
                _exitCode = 504;
            }
        }
    }
    return ret;
}


/*_____PRIVATE_FUNCTIONS_____*/

char **Cgi::_createEnv() {
	map_strstr mapEnv;

	const char *methods[] = {"DELETE", "GET", "POST", NULL};
	mapEnv.insert(std::make_pair("REQUEST_METHOD", methods[_request.getMethod()]));

    std::string pwd(_executablePath);
    pwd.erase(pwd.find_last_of('/'), pwd.size());
	mapEnv.insert(std::make_pair("PWD", pwd));

	std::stringstream content_length;
	content_length << _request.getContentLength();
	mapEnv.insert(std::make_pair("CONTENT_LENGTH", content_length.str()));

    std::stringstream port;
	port << _route.getServer()->getPort();
	mapEnv.insert(std::make_pair("SERVER_PORT", port.str()));

	if (_request.getMethod() != POST)
		mapEnv.insert(std::make_pair("QUERY_STRING", _request.getQuery()));

	mapEnv.insert(std::make_pair("CONTENT_TYPE", _request.getHeader("Content-Type")));
	mapEnv.insert(std::make_pair("SERVER_NAME", _route.getServer()->getServerNames().front()));
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


/* _____GETTERS_____ */

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


/* _____OPERATORS_____ */

Cgi & Cgi::operator=(Cgi const &op) {
    _request = op._request;
    _route = op._route;
    _fileToExec = op._fileToExec;
    _executablePath = op._executablePath;
    _envp = op._envp;
    _exitCode = op._exitCode;
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


/* _____EXCEPTIONS_____ */

const   char* Cgi::PipeException::what() const throw() {
    return "CgiException: pipe() failed.";
}

const   char* Cgi::ForkException::what() const throw() {
    return "CgiException: fork() failed.";
}

const   char* Cgi::NotCgiException::what() const throw() {
    return "";
}

