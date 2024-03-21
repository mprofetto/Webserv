/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 21:08:23 by nesdebie          #+#    #+#             */
/*   Updated: 2024/03/21 13:57:28 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cgi.hpp"

Cgi::Cgi() {
}

Cgi::Cgi(Request const &request, Route const &route) : _request(request), _route(route), _envp(NULL), _exitCode(500) {
	if (!_route.getCgi())
		throw NotCgiException();
	_filePath =  "." + _request.getPath(); // fichier a executer
	_fileExe = _route.getPath(); // localisation de l'executable
}

Cgi::~Cgi() {
	if (_envp) {
		for (size_t i = 0; _envp[i]; i++)
			delete[] _envp[i];
	 	delete[] _envp;		
	}
}

Cgi::Cgi(Cgi const &copy) {
    *this = copy;
}

/* ----- FUNCTIONS ----- */

void Cgi::executeCgi() {
    std::string extension = _getFileExtension(_filePath);
    
    if (extension == ".py" || extension == ".pl") {
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
        if (pid == 0) {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            
			if (!_request.getHeaders().empty())
				_envp = _createEnv();
			
            if (extension == ".py") {
                const char *exec = "/usr/bin/python3";
                char const *args[3] = {"python3", _filePath.c_str(), NULL};
                execve(exec, const_cast<char *const *>(args), _envp);            
            }

            if (extension == ".pl") {
                const char *exec = "/usr/bin/perl";
                char const *args[3] = {"perl",  _filePath.c_str(), NULL};
                execve(exec, const_cast<char *const *>(args), _envp);
            }
            std::cerr << "Error executing CGI." << std::endl;
                exit(EXIT_FAILURE);
        } else {
            close(pipefd[1]);
            char buffer[1024];
            int status;
            ssize_t bytesRead;
            while ((bytesRead = read(pipefd[0], buffer, 1024)) > 0) {
                std::cout.write(buffer, bytesRead);
            }
            close(pipefd[0]);
            waitpid(pid, &status, 0);
            if (!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS)
                std::cerr << "Child process exited with an error." << std::endl;
            else
                _exitCode = 200;
        }
    } else {
       throw UnsupportedExtensionException();
    }
}

char **Cgi::_createEnv() {
	map_strstr mapEnv;
	const char *methods[] = {"DELETE", "GET", "POST", NULL};
	mapEnv.insert(std::make_pair("REQUEST_METHOD", methods[_request.getMethod()]));
	mapEnv.insert(std::make_pair("PWD", ""));// TODO
	if (_request.getMethod() != POST)
		mapEnv.insert(std::make_pair("QUERY_STRING", "")); // TODO
	mapEnv.insert(std::make_pair("CONTENT_TYPE", _request.getHeader("Content-Type").c_str()));
	std::stringstream content_length;
	content_length << _request.getContentLength();
	mapEnv.insert(std::make_pair("CONTENT_LENGTH", content_length.str().c_str()));
	mapEnv.insert(std::make_pair("SERVER_NAME", "")); // server.getName()
	mapEnv.insert(std::make_pair("SERVER_PORT", ""));// port.getPort() en appliquant le meme procede que pour content length
	mapEnv.insert(std::make_pair("SCRIPT_NAME", _request.getPath().c_str()));
	mapEnv.insert(std::make_pair("PATH_INFO", _request.getPath().c_str()));
	mapEnv.insert(std::make_pair("SERVER_PROTOCOL", _request.getHttpVersion().c_str()));

    char **ret = new char *[mapEnv.size() + 1];
    if (!ret)
        return NULL;
	int i = 0;
    for (map_strstr::iterator it = mapEnv.begin(); it != mapEnv.end(); it++)
    {
        std::string tmp = it->first + "=" + it->second;
        ret[i] = new char[tmp.size() + 1];
		if (!ret[i]) {
			for (size_t j = i; j >= 0; --j)
				delete[] ret[j];
			delete[] ret;
			return NULL;
		}
        strcpy(ret[i], tmp.c_str());
        i++;
    }
    ret[i] = NULL;
	return ret;
}

std::string Cgi::_getFileExtension(const std::string& _filePath) {
    size_t dotPos = _filePath.find_last_of('.');
    if (dotPos != std::string::npos) {
        return _filePath.substr(dotPos);
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

std::string Cgi::getFilePath() const {
	return _filePath;
}

std::string Cgi::getFileExe() const {
	return _fileExe;
}

char** Cgi::getEnvp() const {
	return _envp;
}


/* ----- OPERATORS ----- */

Cgi & Cgi::operator=(Cgi const &op) {
    _request = op._request;
    _route = op._route;
    _filePath = op._filePath;
    _fileExe = op._fileExe;
    _envp = op._envp;
    return *this;
}

std::ostream & operator<<(std::ostream &o, Cgi const &obj) {
	o << "Request:\n" << obj.getRequest() << "\n\n";
	o << "Executable:\n|" << obj.getFileExe() << "|\n";
	o << "File to execute:\n|" << obj.getFileExe() << "|\n\n";
    if (obj.getEnvp())
	{
		char **tmp = obj.getEnvp();
		for(size_t i = 0; tmp[i]; i++)
			o << tmp[i] << "\n";
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

const   char* Cgi::UnsupportedExtensionException::what() const throw() {
    return "CgiException: unsupported extention";
}
