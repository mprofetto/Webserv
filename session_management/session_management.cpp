/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session_management.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 09:50:38 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/16 15:19:24 by mprofett         ###   ########.fr       */
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

std::string	generateExpirationDate(time_t duration)
{
	time_t				ltime;

	time(&ltime);
	ltime += duration;

	std::stringstream	ss(asctime(gmtime(&ltime)));
	std::string	weekDay;
	std::string	day;
	std::string	month;
	std::string	year;
	std::string	hours;

	std::getline(ss, month, ' ');
	std::getline(ss, day, ' ');
	std::getline(ss, hours, ' ');
	std::getline(ss, year, ' ');
	std::string	result = weekDay + ", " + day + " "+ month + " " + year + hours + " GMT";
	result.replace(result.find("\n"), 1, " ");
	return (result);
}
