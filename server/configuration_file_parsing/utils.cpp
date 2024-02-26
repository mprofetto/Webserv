/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 10:44:54 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/22 14:08:49 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../TcpListener.hpp"

std::list<std::string>	TcpListener::popFrontToken(std::list<std::string> token_list)
{
	token_list.pop_front();
	if (token_list.empty() == true)
		throw confFileMisconfiguration();
	return (token_list);
}

void				TcpListener::isDigit(std::string nbr) const
{
	for (unsigned int i = 0; i < nbr.size(); i++)
	{
		if (isdigit(*(nbr.c_str() + i)) == 0)
			throw confFileMisconfiguration();
	}
}
