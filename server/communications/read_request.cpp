/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 14:22:20 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/22 17:03:46 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../TcpListener.hpp"
# include "IncompleteRequest.hpp"

void	TcpListener::readRequest(int client_socket)
{
	char		buffer[this->_buffer_max];
	int			bytesReceveid = recv(client_socket, buffer, this->_buffer_max, 0);

	if (bytesReceveid < 0)
	{
		std::cerr << "Error on reading request" << std::endl;
		return;
	}
	else if (bytesReceveid == 0)
	{
		FD_CLR(client_socket, &this->_read_master_fd);
		close(client_socket);
		return;
	}
	std::string	raw_request(buffer, bytesReceveid);
	std::cout << "\n\nRAW REQUEST IS \n" << raw_request << std::endl;
	if (this->incompleteRequestIsAlreadyStored(client_socket) == true)
		this->_incomplete_requests.find(client_socket)->second.appendContent(raw_request);
	else
	{
		IncompleteRequest	new_request(raw_request);

		this->_incomplete_requests.insert(std::pair<int, IncompleteRequest>(client_socket, new_request));
	}
	if (this->_incomplete_requests.find(client_socket)->second.requestIsCompleted() == true)
		registerRequestAsPending(client_socket);
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
	Request request(this->_incomplete_requests.find(client_socket)->second._header, this->_incomplete_requests.find(client_socket)->second._body);

	this->_pending_request = request;
	this->_incomplete_requests.erase(client_socket);
	FD_SET(client_socket, &this->_write_master_fd);
}
