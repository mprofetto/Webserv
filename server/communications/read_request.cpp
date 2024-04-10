/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 14:22:20 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/10 09:36:17 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../TcpListener.hpp"
# include "IncompleteRequest.hpp"

void	TcpListener::readRequest(int client_socket)
{
	std::cout << "Reading new request\n";

	char		buffer[this->_buffer_max + 1];
	int			bytesReceveid = recv(client_socket, buffer, this->_buffer_max, 0);
	std::string	raw_request(buffer, bytesReceveid);

	int	i = 0;
	while (i < bytesReceveid)
	{
		std::cout << "i: " << (int)buffer[i] << std::endl;
		i++;
	}

	if (bytesReceveid <= 0)
	{
		close(client_socket);
		FD_CLR(client_socket, &this->_read_master_fd);
	}
	else
	{
		if (this->incompleteRequestIsAlreadyStored(client_socket) == true)
		{
			this->_incomplete_requests.find(client_socket)->second.appendContent(raw_request);
			std::cout << "Header is: " << this->_incomplete_requests.find(client_socket)->second._header << std::endl;
		}
		else
		{
			IncompleteRequest	new_request(raw_request);

			this->_incomplete_requests.insert(std::pair<int, IncompleteRequest>(client_socket, new_request));
		}
		if (this->_incomplete_requests.find(client_socket)->second.requestIsCompleted() == true)
			registerRequestAsPending(client_socket);
	}
}

bool	TcpListener::incompleteRequestIsAlreadyStored(int socket)
{
	std::map<int, IncompleteRequest>::iterator	it;

	it = this->_incomplete_requests.find(socket);
	if (it == this->_incomplete_requests.end())
		return (false);
	return (true);
}

void	TcpListener::registerRequestAsPending(int client_socket)
{
	std::string	header = this->_incomplete_requests.find(client_socket)->second._header;
	std::string	body = this->_incomplete_requests.find(client_socket)->second._body;
	std::string	raw_request = header + body;

	std::cout << "Content lenght in Listener is " << this->_incomplete_requests.find(client_socket)->second._content_lenght << " ";
	std::cout << "Body size in Listener is " << body.size() << std::endl;

	Request	request(raw_request);

	this->_pending_request = request;
	this->_incomplete_requests.erase(client_socket);
	FD_SET(client_socket, &this->_write_master_fd);
}
