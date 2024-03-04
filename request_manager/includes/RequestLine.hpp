/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 20:28:20 by nesdebie          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/02/28 17:21:56 by nesdebie         ###   ########.fr       */
=======
/*   Updated: 2024/02/29 12:24:25 by nesdebie         ###   ########.fr       */
>>>>>>> 333fa626db11e8f056a3eabc8aa985f52339e39d
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
    RequestLine(int const &method, std::string const &path, std::string const &http_version, std::string const &methodName);
    RequestLine(RequestLine const &copy);
    ~RequestLine();

    RequestLine& operator=(RequestLine const &op);
    
    int getMethod() const;
    std::string getPath() const;
    std::string getHTTPVersion() const;
    std::string getNotValid() const;
};

std::ostream & operator<<(std::ostream &o, RequestLine const &obj);

#endif

