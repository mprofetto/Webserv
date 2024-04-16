/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 09:43:54 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/16 15:44:32 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/*

Je dois m'occuper des erreurs d'ouverture de fichier.
Un code d'erreur doit me parvenir a l'avance depuis le parsing. par defaut,
mon objet reponse est set a 200. dans ce cas, pas de probleme


Quel PATH ?
Comment compiler le CSS ?

*/

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

	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		std::cout << strerror(errno) << std::endl;
	}
	return (0);
}

// int main(void)
// {
// 	std::cout << generateExpirationDate() << "\n";
// }
