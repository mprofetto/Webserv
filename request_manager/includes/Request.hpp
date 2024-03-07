/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 11:08:01 by nesdebie          #+#    #+#             */
/*   Updated: 2024/03/07 15:04:28 by nesdebie         ###   ########.fr       */
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
# define CONTENT_LENGTH_MAX 2000000000

class RequestLine;

#define map_strstr std::map<std::string, std::string>
#define vec_str std::vector<std::string>

class Request {
    private:
        std::string _raw;
        RequestLine _req;
        map_strstr  _headers;
        std::string _body;
        bool        _complete;
        size_t      _content_length;

        void        _parseRequest(std::string const &request);
        vec_str     _vectorSplit(std::string str, char sep);
        std::string _strtrim(std::string & s);

    public:
        Request();
        Request(std::string &request);
        Request(Request const &copy);
        ~Request();

        Request     &operator=(Request const &op);

        void        catToBody(std::string &str);

        std::string getRaw() const;
        RequestLine getRequestLine() const;
        std::string getHeader(std::string const &name);
        map_strstr  getHeaders() const;
        std::string getBody() const;
        bool        getComplete() const;
        int         getContentLenght() const;
  
        class	ContentLengthException : public std::exception {
            public:
                const char *what() const throw();
		};
};

std::ostream        &operator<<(std::ostream &o, Request const &obj);

#endif