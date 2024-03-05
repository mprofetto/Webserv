/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 11:08:01 by nesdebie          #+#    #+#             */
/*   Updated: 2024/03/05 13:43:10 by nesdebie         ###   ########.fr       */
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

# include "./RequestLine.hpp"
# include "../../server/Server.hpp"
//# include "../../webserv.hpp"
# define CONTENT_LENGTH_MAX 2000000000

class RequestLine;

class Request {
    private:
        std::string _raw;
        RequestLine _req;
        std::map<std::string, std::string>  _headers;
        std::string _body;
        bool _complete;
        int _content_length;

    public:
        Request();
        Request(std::string & req);
        Request(Request const &copy);
        ~Request();

        Request & operator=(Request const &op);

        std::string getRaw() const;
        RequestLine getRequestLine() const;
        std::string getHeader(std::string const &name);
        std::map<std::string, std::string> getHeaders() const;
        std::string getBody() const;
        bool getComplete() const;
        int getContentLenght() const;

        void catToBody(std::string & str);
        std::vector<std::string> vectorSplit(std::string str, char sep);
        void setData(std::string head, std::string val);
        std::string ft_strtrim(std::string & s);

		class	HeaderNotFoundException : public std::exception {
            public:
                const char *what() const throw();
		};
        class	ContentLengthException : public std::exception {
            public:
                const char *what() const throw();
		};
};

std::ostream & operator<<(std::ostream &o, Request const &obj);

#endif