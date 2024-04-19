/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 21:08:23 by nesdebie          #+#    #+#             */
/*   Updated: 2024/04/19 14:53:04 by achansar         ###   ########.fr       */
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
	_executablePath = _route.getPath();
    if (!_request.getHeaders().empty())
        _envp = _createEnv();
}

Cgi::~Cgi() {
    if (_envp)
        _freeArray(_envp, -1);
}

Cgi::Cgi(Cgi const &copy) {
    *this = copy;
}

// std::string Cgi::executeCgi() {
//     std::string ret = "";

//     if (access(_fileToExec.c_str(), F_OK) != 0)
//         throw FileNotFoundException();

//     std::string extension = _getFileExtension(_fileToExec, '.');
//     if (!extension.size())
//         throw UnsupportedExtensionException();

//     if (_route.getExtension() != extension || (extension != ".py" && extension != ".pl"))
//         throw UnsupportedExtensionException();

//     int pipefd[2];
//     if (pipe(pipefd) == -1) {
//         throw PipeException();
//     }

//     int fdin = dup(STDIN_FILENO);
//     int fdout = dup(STDOUT_FILENO);

//     pid_t pid_execve = fork();
//     if (pid == -1) {
//         close(pipefd[0]);
//         close(pipefd[1]);
//         close(fdin);
//         close(fdout);
//         throw ForkException();
//     }

//     if (pid == 0) {
//         close(pipefd[0]);
//         dup2(pipefd[1], STDOUT_FILENO);
//         close(pipefd[1]);

//         const char *exec = _executablePath.c_str();
//         std::string exe = _getFileExtension(_executablePath, '/');
//         char const *args[3] = {exe.c_str(), _fileToExec.c_str(), NULL};
//         execve(exec, const_cast<char *const *>(args), _envp);
//         std::cerr << "Error executing CGI." << std::endl;
//         std::exit(EXIT_FAILURE);
//     } else {
//         pid_t pid_timeout = fork();
//         if (pid_timeout == -1) {
//             close(pipefd[0]);
//             close(pipefd[1]);
//             _envp = NULL;
//             throw ForkException();
//         }
//         if (!pid_timeout) {
//             struct timeval tv;
//             tv.tv_sec = CGI_pid_timeout;
//             tv.tv_usec = 0;
//             select(0, NULL, NULL, NULL, &tv);
//             kill(pid, SIGTERM);
//             std::exit(0);
//         } else {
//             close(pipefd[1]);

//             size_t post;
//             if (_request.getMethod() == POST) {
//                 post = write(pipefd[1], _request.getQuery().c_str(), _request.getQuery().size());
//             }

//             int status;
//             int bytesRead;
//             int result = waitpid(pid, &status, 0);
//             if (result == 0) {
//                _exitCode = 504;
//                 return "";
//             }
//             kill(pid_timeout, SIGTERM);
//             char buffer[1024];
//             while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
//                 ret.append(buffer, bytesRead);
//             }
//             close(pipefd[0]);

//             dup2(fdin, STDIN_FILENO);
//             dup2(fdout, STDOUT_FILENO);

//             if (status || (_request.getMethod() == POST && post != _request.getQuery().size())) {
//                 std::cerr << "Child process exited with an error." << std::endl;
//             }
//         }
//     }
//     close(fdin);
//     close(fdout);

//     _exitCode = 200;
//     return ret;
// }

std::string Cgi::executeCgi() {
    std::string ret;

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        throw PipeException();
    }

    int fdin = dup(STDIN_FILENO);
    int fdout = dup(STDOUT_FILENO);
    pid_t pid_execve = fork();
    if (pid_execve == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        close(fdin);
        close(fdout);
        throw ForkException();
    }
    if (pid_execve == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        if (extension == ".py") {
            const char *exec = "/usr/bin/python3";
            char const *args[3] = {"python3", _fileToExec.c_str(), NULL};
            execve(exec, const_cast<char *const *>(args), _envp);
        }

        if (extension == ".pl") {
            const char *exec = "/usr/bin/perl";
            char const *args[3] = {"perl",  _fileToExec.c_str(), NULL};
            execve(exec, const_cast<char *const *>(args), _envp);
        }
        std::cerr << "Error executing CGI." << std::endl;
        std::exit(500);
    } else if (pid_execve > 0) {
        pid_t pid_timeout = fork();
        if (pid_timeout == -1) {
            close(pipefd[0]);
            close(pipefd[1]);
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
            close(pipefd[1]);

            size_t post;
            if (_request.getMethod() == POST) {
                post = write(pipefd[1], _request.getQuery().c_str(), _request.getQuery().size());
            }

            int status;
            waitpid(pid_execve, &status, 0);
            if (WIFEXITED(status)) {
                kill(pid_timeout, SIGTERM);
                int exitStatus = WEXITSTATUS(status);
                if (exitStatus == 0) {
                    _exitCode = 200;
                    char buffer[256];
                    ssize_t bytesRead;
                    while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
                        ret.append(buffer, bytesRead);
                    }
                    close(pipefd[0]);

                    dup2(fdin, STDIN_FILENO);
                    dup2(fdout, STDOUT_FILENO);
                } else {
                    _exitCode = 500;
                }
            } else {
                _exitCode = 504;
                if (_request.getMethod() == POST && post != _request.getQuery().size()) {
                    std::cerr << "Child process exited with an error." << std::endl;
                }
            }
            waitpid(pid_timeout, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 504) {
                _exitCode = 504;
                return "";
            }
        }
    }
    close(fdin);
    close(fdout);
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
    return "CgiException: not a CGI.";
}

const   char* Cgi::FileNotFoundException::what() const throw() {
    return "CgiException: file not found.";
}

const   char* Cgi::UnsupportedExtensionException::what() const throw() {
    return "CgiException: unsupported extention.";
}

const   char* Cgi::WaitpidException::what() const throw() {
    return "CgiException: Waitpid() failed.";
}
