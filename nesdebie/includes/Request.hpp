/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 11:08:01 by nesdebie          #+#    #+#             */
/*   Updated: 2024/02/20 11:25:40 by nesdebie         ###   ########.fr       */
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
//# include "Route.hpp"
//# include "Server.hpp"
# include "../../server/Route.hpp" // TO SWITCH ONCE MERGED
# include "../../server/Server.hpp" //

class RequestLine;
class Route;
class Server;


class Request{
    private:
        std::string _raw;
        RequestLine _req;
        std::map<std::string, std::string>  _headers;
        std::string _body;

    public:
        Request(std::string & req); //TEMP a retirer quand Route et Server achevés
        //Request(std::string & req, Route & route, Server & server); uncomment once fixed
        Request(Request const & copy);
        ~Request();

        std::vector<std::string> vectorSplit(std::string str, char sep);
        void setData(std::string head, std::string val);
        std::string ft_strtrim(std::string & s);
    
        std::string getRaw() const;
        RequestLine getRequestLine() const;
        std::string getHeader(std::string const & name);
        std::map<std::string, std::string> getHeaders() const;
        std::string getBody() const;

        Request & operator=(Request const & op);
};

std::ostream &  operator<<(std::ostream & o, Request const & obj);

#endif