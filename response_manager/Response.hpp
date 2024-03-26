/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:42:45 by achansar          #+#    #+#             */
/*   Updated: 2024/03/26 11:42:49 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <map>
#include "../request_manager/includes/Request.hpp"
#include "../server/Server.hpp"

class Server;
class Response {

    public:

    // CONSTRUCTORS
        Response(Server* server, int statusCode, Request* request, const int socket);
        ~Response();

    // MEMBER FUNCTIONS
        std::string     getBody();
        void            buildResponse(Route* route);
        // void            buildGetResponse(Request request);
        void            buildPostResponse(Request request);
        void            buildErrorResponse();
        std::string     getHeaders(const int s);
        std::string     getReason(int sc);
        std::string     getMimeType();
        void	        getFullPath(Route *route, std::string uri);
        int             fileTransfer();
        std::string     extractExtension(std::string uri);
        std::string     extractFileName();
        int             sendFile();
        int             receiveFile(); 

    // GET & SET
        std::string     getResponse();
        std::string     getPath();
        int             getStatusCode();
        
        void            setPath(std::string& str);
        void            setErrorPath(std::string& str);

    private:
        const int                           _clientSocket;
        const int                           _method;
        int                                 _statusCode;
        std::string                         _path;
        std::string                         _errorPath;
        std::string                         _responseLine;
        std::string                         _statusLine;
        std::string                         _extension;
        std::string                         _headers;
        std::string                         _body;
        Server*                             _server;
        Request*                            _request;
};

enum statusCode {
    OK                      = 200,
    BAD_REQUEST             = 400,
    NOT_FOUND               = 404,
    SERVER_ERROR            = 500,
    BAD_GATEWAY             = 502
};

#endif
