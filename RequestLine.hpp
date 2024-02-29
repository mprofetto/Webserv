/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 14:06:48 by achansar          #+#    #+#             */
/*   Updated: 2024/02/19 14:11:29 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTLINE_HPP
# define REQUESTLINE_HPP

# include <string>
# include <iostream>
# include <fstream>

enum {DELETE, GET, POST, PUT, ERROR}; //PUT not mandatory

class RequestLine {
private:
    int _method;
    std::string _path;
    std::string _http_version;

public:
    RequestLine();
    RequestLine(const int&  method, const std::string& path, const std::string& http_version);
    RequestLine(const RequestLine& other);
    ~RequestLine();

    RequestLine& operator=(const RequestLine& other);
    
    int getMethod() const;
    std::string getPath() const;
    std::string getHTTPVersion() const;
};

std::ostream &  operator<<(std::ostream & o, RequestLine const & obj);

#endif