/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 21:03:56 by nesdebie          #+#    #+#             */
/*   Updated: 2024/03/04 19:39:23 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

#include "Request.hpp"

#include <map>
#include <vector>

class Cgi {
	private:
		FILE* _body_file;
		pid_t _cgi_pid;
		char** _env;
		std::string	_file_path;
		std::string	_bin_path;
		std::map<std::string, std::string> _cgi_env;
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
		void 	setData(std::string head, std::string val);
		std::map<std::string, std::string> fillEnvs(Request & request);
		int		getStdout() const;
		int		getStderr() const;
		pid_t	getPid() const;
};

#endif