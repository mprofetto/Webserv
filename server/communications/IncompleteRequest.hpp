/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IncompleteRequest.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 14:46:06 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/10 09:05:41 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCOMPLETEREQUEST_HPP
# define INCOMPLETEREQUEST_HPP

# include <string>

class IncompleteRequest
{
	public:
		unsigned long	_content_lenght;
		bool			_headerIsCompleted;
		bool			_bodyIsCompleted;
		std::string		_header;
		std::string		_body;
		std::string		_raw;

		IncompleteRequest(std::string raw_request);
		~IncompleteRequest();

		void	appendContent(std::string content_to_add);
		bool	requestIsCompleted(void);

	private:

		void	appendToHeader(std::string content_to_add);
		void	findHeaderEnd(void);
		void	getContentLength(void);
};

#endif
