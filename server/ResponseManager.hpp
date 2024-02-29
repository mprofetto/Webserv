/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseManager.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:42:45 by achansar          #+#    #+#             */
/*   Updated: 2024/02/19 17:17:51 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEMANAGER_HPP
#define RESPONSEMANAGER_HPP

#include <iostream>
#include <map>

// MUST BUILD :
//      - status line [ HTTPversion  -  response code  -  reason]
//      - headers (which ones ?)
//      - opt message body


class ResponseManager {

    public:
        ResponseManager(const int statusCode);
        ~ResponseManager();

        void getResponseLine();

    private:
        const int                           _statusCode;
        std::string                         _responseLine;
        std::string                         _statusLine;
        std::map<std::string, std::string>  _headers;//           utile ?
        std::string                         _body;
};

enum statusCode {
    OK                      = 200,
    BAD_REQUEST             = 400,
    NOT_FOUND               = 404,
    SERVER_ERROR            = 500,
    BAD_GATEWAY             = 502
};

#endif
