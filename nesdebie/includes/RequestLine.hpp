/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 20:28:20 by nesdebie          #+#    #+#             */
/*   Updated: 2024/02/20 14:07:39 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_LINE_HPP
# define REQUEST_LINE_HPP

# include <string>
# include <iostream>
# include <fstream>

enum {DELETE, GET, POST, UNVALID};

class RequestLine {
private:
    int _method;
    std::string _path;
    std::string _http_version;
    std::string _not_valid;

public:
    RequestLine();
    RequestLine(const int&  method, const std::string& path, const std::string& http_version, const std::string& methodName);
    RequestLine(const RequestLine& copy);
    ~RequestLine();

    RequestLine& operator=(const RequestLine& op);
    
    int getMethod() const;
    std::string getPath() const;
    std::string getHTTPVersion() const;
    std::string getNotValid() const;
};

std::ostream &  operator<<(std::ostream & o, RequestLine const & obj);

#endif

