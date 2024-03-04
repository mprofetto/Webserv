/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 21:03:56 by nesdebie          #+#    #+#             */
/*   Updated: 2024/03/04 20:28:01 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include "Request.hpp"

# include <map>
# include <vector>
# include <string>
# include <cstring>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

class Cgi {
	private:
		FILE* _body_file;
		pid_t _cgi_pid;
		char** _env;
		std::string	_file_path;
		std::string	_bin_path;
		std::map<std::string, std::string> _map;
		std::vector<std::string> _vec;
		int _cgi_stdout;
		int _cgi_stderr;

	public:
		Cgi(Request & request);
		Cgi(Cgi const &copy);
		~Cgi();

		Cgi &operator=(const Cgi &op);

		bool	validateBinPath();
		bool	execute(Request &request);
		int		wait();
		bool	setupFiles();
		bool 	create_body_file(Request &request);
		void 	setData(std::string &head, std::string &val);
		void	fillEnvs(Request &request);
		int		getStdout() const;
		int		getStderr() const;
		pid_t	getPid() const;
};

#endif