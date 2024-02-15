/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 09:43:54 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/15 11:35:15 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	TcpListener server("127.0.0.1", 80);

	try
	{
		server.init();
		server.run();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		std::cout << strerror(errno) << std::endl;
	}
	return (0);
}
