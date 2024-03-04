/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 21:08:23 by nesdebie          #+#    #+#             */
/*   Updated: 2024/03/04 19:50:57 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>

Cgi::Cgi(Request & request) : _env(NULL)
{
	_file_path = request.getRequestLine().getPath();
	_bin_path = ""; // TODO => path to the CGI script being executed
	_cgi_pid = -1;
	_body_file = NULL;
	_cgi_stdout = -1;
	_cgi_stderr = -1;
    _cgi_env.clear();
	_cgi_env = fillEnvs(request);
}

Cgi::Cgi(Cgi const &copy) {
	*this = copy;
}

Cgi::~Cgi() {
}

 /*----- OPERATORS ----- */
 
Cgi &Cgi::operator=(Cgi const &op) {
	if (this != &op)
	{
		_file_path = op._file_path;
		_bin_path = op._bin_path;
		_env = op._env;
		_cgi_pid = op._cgi_pid;
		_body_file = op._body_file;
		_cgi_stdout = op._cgi_stdout;
		_cgi_stderr = op._cgi_stderr;
		_cgi_env = op._cgi_env;
	}
	return *this;
}


/*----- FUNCTIONS -----*/

std::map<std::string, std::string> Cgi::fillEnvs(Request & request) {
/* Variables fondamentales pour majorite CGI */
	char **methods = NULL;
	methods[0] = "DELETE";
	methods[1] = "GET";
	methods[2] = "POST";
	methods[3] = NULL;
	this->setData("REQUEST_METHOD", methods[request.getRequestLine().getMethod()]);
	this->setData("CONTENT_TYPE", request.getHeader("Content-Type"));
	std::stringstream ss;
	ss << strlen(request.getBody().c_str());
	this->setData("CONTENT_LENGTH", ss.str()); // length of the body in bytes
	this->setData("QUERY_STRING", "");  // TODO => query string portion of the URL
	this->setData("SCRIPT_NAME", _bin_path); //TODO => path to the CGI script being executed
	this->setData("REMOTE_ACCESS", ""); //TODO =>  IP address of the client making the request 
	this->setData("HTTP_USER_AGENT", request.getHeader("User-Agent"));

	/* Autres variables */
	this->setData("REQUEST_URI", request.getRequestLine().getPath());
	this->setData("SERVER_NAME", request.getHeader("Host"));
	this->setData("SERVER_SOFTWARE", "Apache/2.4.41"); // NOT SURE (nom et version du serveur web qui execute le script)
	this->setData("REDIRECT_STATUS", "200");
	this->setData("HTTP_CONNECTION", request.getHeader("Connection"));
	this->setData("HTTP_HOST", request.getHeader("Host"));
	this->setData("HTTP_COOKIE", request.getHeader("Cookie")); // BONUS ?
	this->setData("HTTP_ACCEPT", request.getHeader("Accept"));
	this->setData("HTTP_VERSION", request.getRequestLine().getHTTPVersion());
	this->setData("SERVER_PROTOCOL", request.getRequestLine().getHTTPVersion());
	this->setData("PATH_INFO", request.getRequestLine().getPath());
	return _cgi_env;
}

void Cgi::setData(std::string head, std::string val) {
    this->_cgi_env.insert(std::make_pair(head, val));
}

bool	Cgi::validateBinPath() {
	struct stat		file_stat;

	memset(&file_stat, 0, sizeof(file_stat));
	int ret = stat(_bin_path.c_str(), &file_stat); 
	if (ret == -1) {
		std::cout << "No such file or directory\n"; // FILE NOT FOUND
		return false;
	}
	if (S_ISDIR(file_stat.st_mode)) {
		std::cout << "Is a directory\n"; // NOT A FILE
		return false;
	}
	if (!(file_stat.st_mode & S_IXUSR))  {
		std::cout << "Permission denied\n"; // FILE NOT EXECUTABLE
		return false;
	}
	return true;
}

bool	Cgi::create_body_file(Request &request) {
	_body_file = tmpfile(); //creates a temp file
	if (_body_file == NULL)
		return false;
	for (size_t i = 0; i < request.getRaw().size(); i++)
		fputc(request.getRaw()[i], _body_file);
	return true;
}

bool	Cgi::setupFiles() {
	_cgi_stdout = fileno(tmpfile());
	if (_cgi_stdout == -1) {
		std::cout << "failed to create a temporary file for the Cgi script output";
		return false;
	}
	_cgi_stderr = fileno(tmpfile());
	if (_cgi_stderr == -1) {
		std::cout << "failed to create a temporary file for the Cgi script error";
		return false;
	}
	return true;	
}

bool	Cgi::execute(Request &request) {
	_cgi_pid = fork();
	if (_cgi_pid == -1) {
		std::cout << "failed to fork";
		return false;
	}
	if (_cgi_pid == 0)
	{
		if (request.getRequestLine().getMethod() == POST)
		{
			rewind(_body_file); // sets the file position to the beginning of the file of the given stream
			if (dup2(fileno(_body_file), STDIN_FILENO) == -1)
				exit(EXIT_FAILURE);
		} 
		if (dup2(_cgi_stdout, STDOUT_FILENO) == -1)
			exit(EXIT_FAILURE);
		if (dup2(_cgi_stderr, STDERR_FILENO) == -1)
			exit(EXIT_FAILURE);
    
        char const *args[3]; // obligatoire a declarer comme ca (ne veut pas compiler sinon)
		args[0] = _bin_path.c_str(); // TODO
		args[1] = _file_path.c_str();
		args[2] = NULL;
		if (execve(args[0], (char* const*)args, _env) == -1)
			exit(EXIT_FAILURE);
	}
	return true;
}

int	Cgi::wait() {
	int		status;

	if (waitpid(_cgi_pid, &status, 0) == -1) {
		std::cout << "failed to wait for the Cgi script";
		return EXIT_FAILURE;
	}
	if (WIFEXITED(status)) {
		if (WEXITSTATUS(status) == EXIT_SUCCESS)
			return EXIT_SUCCESS;
	}
    std::cout << "the Cgi script exited with an error";
	return EXIT_FAILURE;
}


 /*----- GETTERS -----*/
 
int	Cgi::getStdout() const {
	return _cgi_stdout;
}

int	Cgi::getStderr() const {
	return _cgi_stderr;
}

pid_t	Cgi::getPid() const {
	return _cgi_pid;
}
