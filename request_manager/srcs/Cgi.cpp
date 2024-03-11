/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 21:08:23 by nesdebie          #+#    #+#             */
/*   Updated: 2024/03/07 16:36:14 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cgi.hpp"

Cgi::Cgi() {
}

Cgi::Cgi(Request & request) : _env(NULL) {
	_file_path = request.getRequestLine().getPath();
	_bin_path = ""; // TODO => path to the CGI script being executed
	_cgi_pid = -1;
	_cgi_stdout = -1;
	_cgi_stderr = -1;
	_map.clear();
	_fillEnvs(request);
}

Cgi::Cgi(Cgi const &copy) {
	*this = copy;
}

Cgi::~Cgi() {
	if (_env) {
		for (size_t i = 0; i < _vec.size(); i++)
			free (_env[i]);
		delete _env;
	}
}

 /*----- OPERATORS ----- */
 
Cgi &Cgi::operator=(Cgi const &op) {
	if (this != &op)
	{
		_file_path = op._file_path;
		_bin_path = op._bin_path;
		_env = op._env;
		_cgi_pid = op._cgi_pid;
		_cgi_stdout = op._cgi_stdout;
		_cgi_stderr = op._cgi_stderr;
		_map = op._map;
	}
	return *this;
}


/*----- FUNCTIONS -----*/

void Cgi::_fillEnvs(Request  &request) {
/* Variables fondamentales pour majorite CGI */
	const char **methods = NULL;
	methods[0] = "DELETE";
	methods[1] = "GET";
	methods[2] = "POST";
	methods[3] = 0;
	this->_setMap("REQUEST_METHOD", methods[request.getRequestLine().getMethod()]);
	this->_setMap("CONTENT_TYPE", request.getHeader("Content-Type").c_str());
	std::stringstream ss;
	ss << strlen(request.getBody().c_str());
	this->_setMap("CONTENT_LENGTH", ss.str().c_str()); // length of the body in bytes
	this->_setMap("QUERY_STRING", "");  // TODO => query string portion of the URL
	this->_setMap("SCRIPT_NAME", _bin_path.c_str()); //TODO => path to the CGI script being executed
	this->_setMap("REMOTE_ACCESS", ""); //TODO =>  IP address of the client making the request 
	this->_setMap("HTTP_USER_AGENT", request.getHeader("User-Agent").c_str());

	/* Autres variables */
	this->_setMap("REQUEST_URI", request.getRequestLine().getPath().c_str());
	this->_setMap("SERVER_NAME", request.getHeader("Host").c_str());
	this->_setMap("SERVER_SOFTWARE", "Apache/2.4.41"); // NOT SURE (nom et version du serveur web qui execute le script)
	this->_setMap("REDIRECT_STATUS", "200");
	this->_setMap("HTTP_CONNECTION", request.getHeader("Connection").c_str());
	this->_setMap("HTTP_HOST", request.getHeader("Host").c_str());
	this->_setMap("HTTP_COOKIE", request.getHeader("Cookie").c_str()); // BONUS ?
	this->_setMap("HTTP_ACCEPT", request.getHeader("Accept").c_str());
	this->_setMap("HTTP_VERSION", request.getRequestLine().getHTTPVersion().c_str());
	this->_setMap("SERVER_PROTOCOL", request.getRequestLine().getHTTPVersion().c_str());
	this->_setMap("PATH_INFO", request.getRequestLine().getPath().c_str());

	for (map_strstr::iterator it = _map.begin(); it != _map.end(); it++) {
			_vec.push_back(it->first + "=" + it->second);
		}
		_env = new char*[_vec.size() + 1];
		if (_env == NULL) {
			std::cout << "failed to allocate memory for the cgi envirenement variables";
			// throw une erreur
		}
		for (size_t i = 0; i < _vec.size(); i++) {
			_env[i] = strdup(_vec[i].c_str());
			if (_env[i] == NULL) {
				for (i; i > 0; --i)
					free(_env[i]);
				delete _env;
				_env = NULL;
				std::cout << "failed to allocate memory for the cgi envirenement variables";
				// throw erreur
			}
		}
		_env[_vec.size()] = NULL;
}

void Cgi::_setMap(const char *head, const char *val) {
    _map.insert(std::make_pair(head, val));
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

bool	Cgi::execute(Request &request) {
	_cgi_pid = fork();
	if (_cgi_pid == -1) {
		std::cout << "failed to fork";
		return false;
	}
	if (_cgi_pid == 0)
	{
		if (request.getRequestLine().getMethod() == POST)
			write(STDIN_FILENO, request.getBody().c_str(), request.getContentLenght());
		if (dup2(_cgi_stdout, STDOUT_FILENO) == -1)
			exit(EXIT_FAILURE);
		if (dup2(_cgi_stderr, STDERR_FILENO) == -1)
			exit(EXIT_FAILURE);
    
        char const *args[3]; // obligatoire a declarer comme ca (ne veut pas compiler sinon)
		args[0] = _bin_path.c_str();
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
			return EXIT_SUCCESS; // 200
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

pid_t Cgi::getPid() const {
	return _cgi_pid;
}

std::string	Cgi::getBinPath() const {
	return _bin_path;
}

std::string	Cgi::getFilePath() const {
	return _file_path;
}

char**	Cgi::getEnv() const {
	return _env;
}
/* ----- OPERATORS ----- */

std::ostream & operator<<(std::ostream &o, Cgi const &obj) {
	o << "bin: " << obj.getBinPath() << "\nfile:" << obj.getFilePath() << std::endl;
    return o;
}
