/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 21:08:23 by nesdebie          #+#    #+#             */
/*   Updated: 2024/04/15 15:29:27 by nesdebie         ###   ########.fr       */
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
    else 
        _fileToExec = "." + _request.getPath() + "?" + _request.getQuery();
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
    //check if the file to execute actually exist
	if (access(_fileToExec.c_str(), F_OK) != 0)
		throw FileNotFoundException();

    //check if the file to exec got an executable extension for python3 or perl
    std::string extension = _getFileExtension(_fileToExec);
    
    if (_route.getExtension() == extension && (extension == ".py" || extension == ".pl")) {
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            throw PipeException();
        }
        pid_t pid = fork();
        if (pid == -1) {
            close(pipefd[0]);
            close(pipefd[1]);
            throw ForkException();
        }

        // CHILD
        if (pid == 0) {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            
			if (!_request.getHeaders().empty())
				_envp = _createEnv();
			
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
                exit(EXIT_FAILURE);
        } else { //PARENT
            std::string tmp = "";
            close(pipefd[1]);
            char buffer[1023];
            int status;
            ssize_t bytesRead;
            while ((bytesRead = read(pipefd[0], buffer, 1023)) > 0) {
                std::cout.write(buffer, bytesRead);
                tmp += buffer;
            }
            close(pipefd[0]);
            waitpid(pid, &status, 0);
            if (!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS)
                std::cerr << "Child process exited with an error." << std::endl;
            else
                _exitCode = 200;
                return tmp;
        }
    } else {
       throw UnsupportedExtensionException();
    }
    return "";
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
	mapEnv.insert(std::make_pair("SERVER_NAME", _route.getServer()->getServerNames().front())); // to check !!
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
    else
        for (int i = 0; i < flag; i++)
            delete[] arr[i];
	delete[] arr;	  
}

std::string Cgi::_getFileExtension(const std::string& _fileToExec) {
    size_t dotPos = _fileToExec.find_last_of('.');
    if (dotPos != std::string::npos) {
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

