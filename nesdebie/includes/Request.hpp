/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 11:08:01 by nesdebie          #+#    #+#             */
/*   Updated: 2024/02/27 12:55:38 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# define SPACE 32

# include <string>
# include <cstring>
# include <sstream>
# include <vector>
# include <iostream>
# include <map>

# include "RequestLine.hpp"
//# include "Server.hpp" // TO UNCOMMENT ONCE MERGED
# include "../../server/Server.hpp" // TO COMMENT ONCE MERGED

class RequestLine;
class Server;

class Request{
    private:
        std::string _raw;
        RequestLine _req;
        int _port;
        std::map<std::string, std::string>  _headers;
        std::string _body;

    public:
        Request(std::string & req, Server * server);
        Request(Request const &copy);
        ~Request();

        Request & operator=(Request const &op);

        int getPort() const;
        std::string getRaw() const;
        RequestLine getRequestLine() const;
        std::string getHeader(std::string const &name);
        std::map<std::string, std::string> getHeaders() const;
        std::string getBody() const;

        std::vector<std::string> vectorSplit(std::string str, char sep);
        void setData(std::string head, std::string val);
        std::string ft_strtrim(std::string & s);
};

std::ostream & operator<<(std::ostream &o, Request const &obj);

#endif