/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 21:03:56 by nesdebie          #+#    #+#             */
/*   Updated: 2024/03/13 12:08:59 by nesdebie         ###   ########.fr       */
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

class Cgi {
private:
    Request		_request;
	Route		_route;
	std::string	_filePath;
	std::string _fileExe;
	char**		_envp;

    std::string _getFileExtension(std::string const &filePath);
	char**		_createEnv();

public:
    Cgi();
	Cgi(Request const &request, Route const &route);
	Cgi(Cgi const &copy);
	~Cgi();

	Cgi     	&operator=(Cgi const &op);
	
    void		executeCgi();

	Request		getRequest() const;
	Route		getRoute() const;
	std::string getFilePath() const;
	std::string getFileExe() const;
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
};

std::ostream	&operator<<(std::ostream &o, Cgi const &obj);

#endif