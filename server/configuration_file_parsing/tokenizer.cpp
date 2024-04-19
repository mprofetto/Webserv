/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:22:51 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/18 23:32:59 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../TcpListener.hpp"

std::string	getNextToken(size_t *start_pos, std::string line)
{
	size_t		end_pos;
	std::string	result;

	*start_pos = line.find_first_not_of(" \t", *start_pos);
	if (*start_pos == std::string::npos)
		return ("");
	if (*(line.c_str() + *start_pos) == '{' || *(line.c_str() + *start_pos) == '}' || *(line.c_str() + *start_pos) == ';')
	{
		result = line.substr(*start_pos, 1);
		*start_pos += 1;
	}
	else
	{
		end_pos = line.find_first_of(" \t{};", *start_pos);
		result = line.substr(*start_pos, end_pos - *start_pos);
		*start_pos = end_pos;
	}
	return (result);
}

std::list<std::string>	TcpListener::tokenizeConfigurationFile(std::string filename)
{
	std::ifstream			myfile;
	std::list<std::string>	result;
	std::string				token = "temp";
	std::string				line;
	size_t					pos = 0;

	std::cout << "\n\n!!!\nFILENAME : " << filename << "\n!!!\n\n";

	myfile.open(filename.c_str());
	if (!myfile)
		throw openFileFailure();
	while (std::getline(myfile, line))
	{
		while (pos != std::string::npos)
		{
			token = getNextToken(&pos, line);
			if (token.empty() == false)
				result.push_back(token);
		}
		pos = 0;
	}
	myfile.close();
	return (result);
}

// DEBUG ONLY
// void	printTokenList(std::list<std::string> list)
// {
// 	unsigned int list_size = list.size();

// 	for (unsigned int i = 0; i < list_size; i++)
// 	{
// 		std::cout << list.front() << std::endl;
// 		list.pop_front();
// 	}
// }
