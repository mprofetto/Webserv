/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 09:43:54 by mprofett          #+#    #+#             */
/*   Updated: 2024/03/04 11:52:25 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	checkArguments(int argc, char **argv)
{
	std::string	filename;
	std::string	extension = ".conf";

	if (argv[1])
		filename = argv[1];
	if (argc < 2)
		std::cout << "Configuration file is needed\n";
	else if (filename.size() < 6)
		std::cout << "Wrong Configuration file name. Configuration should have a name and be ended by \'.conf\'\n";
	else if (extension.compare(filename.substr(filename.size() - 5, 5)) != 0)
		std::cout << "Wrong Configuration file name. Configuration should have a name and be ended by \'.conf\'\n";
	else
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	if (checkArguments(argc, argv) == 1)
		return (1);
	try
	{
		TcpListener server(argv[1]);

		server.initTcpListener();
		server.runTcpListener();

		// TEMP
		// Response response(200);

		// response.getResponseLine();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		std::cout << strerror(errno) << std::endl;
	}
	return (0);
}
