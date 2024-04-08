/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 14:22:20 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/08 11:13:15 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../TcpListener.hpp"

std::string		convert_buffer_to_string(int size, char *buffer)
{
	std::string				result;
	char					str[size + 1];
	int						i = 0;

	while (i < size)
	{
		str[i] = buffer[i];
		i++;
	}
	str[size] = 0;
	result = str;
	return (result);
}

void	TcpListener::create_new_incoming_request(int client_socket, int size, char *buffer)
{
	std::string	new_content;

	new_content = convert_buffer_to_string(size, buffer);

	this->_incomplete_requests.insert(std::pair<int, std::string>(client_socket, new_content));
}

void	TcpListener::update_incoming_request(int client_socket, int size, char *buffer)
{
	std::string	oldStr = this->_incomplete_requests.find(client_socket)->second;
	std::string newStr = convert_buffer_to_string(size, buffer);

	this->_incomplete_requests.find(client_socket)->second = oldStr + newStr;
}

void	TcpListener::register_new_pending_request(int client_socket, char *buffer)
{
	std::string	raw_request(buffer);

	if (this->_incomplete_requests.find(client_socket) == this->_incomplete_requests.end())
	{
		Request 	request(raw_request);

		this->_pending_request = request;
	}
	else
	{
		update_incoming_request(client_socket, strlen(buffer), buffer);
		Request	request(this->_incomplete_requests.find(client_socket)->second);

		this->_pending_request = request;
		this->_incomplete_requests.erase(client_socket);
	}
	std::cout << "Request Body: " << this->_pending_request.getRaw() << std::endl;
	FD_SET(client_socket, &this->_write_master_fd);
}

void	TcpListener::readRequest(int client_socket)
{
	std::cout << "Reading new request\n";
	char	buffer[this->_buffer_max];
	int		bytesReceveid;

	memset(&buffer, 0, this->_buffer_max);
	bytesReceveid = recv(client_socket, buffer, this->_buffer_max, 0);
	if (bytesReceveid <= 0)
	{
		close(client_socket);
		FD_CLR(client_socket, &this->_read_master_fd);
	}
	else if ((bytesReceveid != this->_buffer_max && buffer[bytesReceveid] == '\0')
		|| (bytesReceveid == this->_buffer_max && buffer[bytesReceveid - 1] == '\0'))
		{
			register_new_pending_request(client_socket, buffer);
			return;
		}
	else
	{
		if (this->isIncompleteRequest(client_socket) == true)
		{
			update_incoming_request(client_socket, bytesReceveid, buffer);
			if (this->_incomplete_requests.find(client_socket)->second.find("\r\n\r\n") != std::string::npos)
			{
				Request	request(this->_incomplete_requests.find(client_socket)->second);

				if (request.getContentLength() == 0)
				{
					this->_pending_request = request;
					this->_incomplete_requests.erase(client_socket);
					FD_SET(client_socket, &this->_write_master_fd);
					return;
				}
				else
				{
					//
					;
				}
			}
		}
		else
			create_new_incoming_request(client_socket, bytesReceveid, buffer);
		this->registerReponse(client_socket, "HTTP/1.1 100 CONTINUE");
	}
}
