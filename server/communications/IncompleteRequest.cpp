/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IncompleteRequest.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 14:49:51 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/10 09:41:25 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IncompleteRequest.hpp"
#include <iostream>

IncompleteRequest::IncompleteRequest(std::string raw_request) :
	_content_lenght(0),
	_headerIsCompleted(false),
	_bodyIsCompleted(false)
{
	this->appendToHeader(raw_request);
}

IncompleteRequest::~IncompleteRequest()
{
	return;
}

void	IncompleteRequest::appendToHeader(std::string content_to_add)
{
	this->_header += content_to_add;
	this->findHeaderEnd();
	if (this->_headerIsCompleted == true)
	{
		this->getContentLength();
		if (this->_content_lenght != 0)
			this->_body = this->_raw.substr(this->_header.size(), std::string::npos);
		if (this->_content_lenght == this->_body.size())
			this->_bodyIsCompleted = true;
	}
}

void	IncompleteRequest::findHeaderEnd(void)
{
	unsigned long	header_end = this->_header.find("\r\n\r\n");

	std::cout << "Header end index is: " << header_end << std::endl;
	if (header_end == std::string::npos)
		return;
	this->_raw = this->_header;
	this->_header = this->_header.substr(0, header_end + 4);
	this->_headerIsCompleted = true;
}

void	IncompleteRequest::getContentLength(void)
{
	unsigned long	content_lenght_index = this->_header.find("Content-Length:");
	unsigned long	value_index;
	unsigned long	value_end;
	std::string		content_lenght;

	if (content_lenght_index == std::string::npos)
		return;
	value_index = this->_header.find_first_of(":", content_lenght_index) + 1;
	value_end = this->_header.find_first_of("\r\n", value_index) - 1;
	content_lenght = this->_header.substr(value_index, value_end - value_index + 1);
	this->_content_lenght = atoi(content_lenght.c_str());
}

void	IncompleteRequest::appendContent(std::string content_to_add)
{
	if (this->_headerIsCompleted == false)
	{
		this->appendToHeader(content_to_add);
	}
	else
	{
		this->_body += content_to_add;
		if (this->_content_lenght == this->_body.size())
			this->_bodyIsCompleted = true;
	}
}

bool	IncompleteRequest::requestIsCompleted(void)
{
	if (this->_headerIsCompleted == true
		&& (this->_content_lenght == 0 || this->_bodyIsCompleted == true))
			return (true);
	return (false);
}
