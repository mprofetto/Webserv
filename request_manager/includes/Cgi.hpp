/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 21:03:56 by nesdebie          #+#    #+#             */
/*   Updated: 2024/03/07 15:28:42 by nesdebie         ###   ########.fr       */
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
		pid_t		_cgi_pid;
		char**		_env;
		std::string	_file_path;
		std::string	_bin_path;
		map_strstr	_map;
		vec_str		_vec;
		int			_cgi_stdout;
		int			_cgi_stderr;

		void	_fillEnvs(Request &request);
		void 	_setMap(const char *head, const char *val);

	public:
		Cgi();
		Cgi(Request & request);
		Cgi(Cgi const &copy);
		~Cgi();

		Cgi &operator=(const Cgi &op);

		bool	validateBinPath();
		bool	execute(Request &request);
		int		wait();

		std::string	getFilePath() const;
		std::string	getBinPath() const;
		int		getStdout() const;
		int		getStderr() const;
		pid_t	getPid() const;
};

std::ostream        &operator<<(std::ostream &o, Cgi const &obj);

#endif