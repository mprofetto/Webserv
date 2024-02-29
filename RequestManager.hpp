/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 14:05:32 by achansar          #+#    #+#             */
/*   Updated: 2024/02/19 15:30:34 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTMANAGER_HPP
# define REQUESTMANAGER_HPP

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
# include "server/Route.hpp" // TO SWITCH ONCE MERGED
# include "server/Server.hpp" //

class RequestLine;
class Route;
class Server;


class RequestManager{
    private:
        std::string _raw;
        RequestLine _req;
        std::map<std::string, std::string>  _headers;
        std::string _body;

    public:
        RequestManager(std::string & req); //TEMP a retirer quand Route et Server achevés
        //RequestManager(std::string & req, Route & route, Server & server); uncomment once fixed
        RequestManager(RequestManager const & copy);
        ~RequestManager();

        std::vector<std::string> vectorSplit(std::string str, char sep);
        void setData(std::string head, std::string val);
        std::string ft_strtrim(std::string & s);
    
        std::string getRaw() const;
        RequestLine getRequestLine() const;
        std::string getHeader(std::string const & name);
        std::map<std::string, std::string> getHeaders() const;
        std::string getBody() const;

        RequestManager & operator=(RequestManager const & op);
};

std::ostream &  operator<<(std::ostream & o, RequestManager const & obj);

#endif