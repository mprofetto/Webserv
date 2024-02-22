/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 14:02:43 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/21 09:56:46 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <list>

template<typename T>
void	clearList(std::list<T> &list)
{
	for(unsigned int i = 0; i < list.size(); ++i)
	{
		delete list.back();
		list.pop_back();
	}
	list.clear();
}

#endif
