/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 20:28:20 by nesdebie          #+#    #+#             */
/*   Updated: 2024/04/08 13:48:58 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_LINE_HPP
# define REQUEST_LINE_HPP

# include <string>
# include <iostream>
# include <fstream>
# include <sys/stat.h>

enum {DELETE, GET, POST, UNVALID};

class RequestLine {
private:
    int         _method;
    std::string _path;
    std::string _http_version;
    std::string _not_valid;
    std::string _query;

public:
    RequestLine();
    RequestLine(int const &method, std::string const &path, std::string const &http_version, std::string const &methodName);
    RequestLine(RequestLine const &copy);
    ~RequestLine();

    RequestLine &operator=(RequestLine const &op);
    
    int         getMethod() const;
    std::string getPath() const;
    std::string getHTTPVersion() const;
    std::string getNotValid() const;
    std::string getQuery() const;
};

std::ostream    &operator<<(std::ostream &o, RequestLine const &obj);

#endif

