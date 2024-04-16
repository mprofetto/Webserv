/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session_management.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 09:50:38 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/12 11:15:09 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/TcpListener.hpp"

std::string	generateSessionId(void)
{
	std::stringstream ss;

	srand(time(NULL) + rand());
	for(int i = 0; i < 64; i++)
	{
		int j = rand() % 127;
		while(j < 32)
			j = rand() % 127;
		ss << char(j);
	}
	return (ss.str());
}
