/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 21:03:56 by nesdebie          #+#    #+#             */
/*   Updated: 2024/04/19 15:09:26 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include "Request.hpp"
# include "../../server/Route.hpp"

# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <cstdlib>

# include <iostream>
# include <map>
# include <string>
# include <vector>
# include <signal.h>

# define CGI_TIMEOUT 3

class Cgi {
private:
    Request		_request;
	Route		_route;
	std::string	_fileToExec;
	std::string _executablePath;
	char**		_envp;
	int			_exitCode;

    std::string _getFileExtension(std::string const &fileToExec, char sep);
	char**		_createEnv();
	void		_freeArray(char **arr, int flag);

public:
    Cgi();
	Cgi(Request const &request, Route const &route);
	Cgi(Cgi const &copy);
	~Cgi();

	Cgi     	&operator=(Cgi const &op);
	
    std::string		executeCgi();

	int			getExitCode() const;
	Request		getRequest() const;
	Route		getRoute() const;
	std::string getFileToExec() const;
	std::string getExecutablePath() const;
	char**		getEnvp() const;

    class	PipeException : public std::exception {
        public:
            const char *what() const throw();
	};
    class	ForkException : public std::exception {
        public:
            const char *what() const throw();
	};
    class	NotCgiException : public std::exception {
        public:
            const char *what() const throw();
	};
    class	UnsupportedExtensionException : public std::exception {
        public:
            const char *what() const throw();
	};
	class	FileNotFoundException : public std::exception {
        public:
            const char *what() const throw();
	};
	class	WaitpidException : public std::exception {
        public:
            const char *what() const throw();
	};
};

std::ostream	&operator<<(std::ostream &o, Cgi const &obj);

#endif