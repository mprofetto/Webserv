/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:42:45 by achansar          #+#    #+#             */
/*   Updated: 2024/04/11 09:44:01 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <map>
#include "../request_manager/includes/Request.hpp"
#include "../server/Server.hpp"
#include "../server/TcpListener.hpp"
#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <filesystem>
#include <sys/stat.h>
#include <dirent.h>

class Server;
class Response {

    public:

    // CONSTRUCTORS
        Response(Server* server, int statusCode, Request* request, const int socket);
        ~Response();

    // MEMBER FUNCTIONS
        void            getBody(bool autodindex);
        void            buildResponse(Route* route);
        void            buildErrorResponse();
        std::string     getHeaders(const int s);
        std::string     getReason(int sc);
        int             fileTransfer();
        int             sendFile();
        int             receiveFile();
        int             deleteFile();
        std::string     extractFileBody(std::string request);
        int             generateAutoindex();

    // UTILS
        void	        getFullPath(Route *route, std::string uri);
        std::string     extractExtension(std::string uri);
        std::string     extractFileName();
        std::string     getMimeType();
        bool            isDirectory(std::string path);

    // GET & SET
        std::string     getResponse();
        std::string     getPath();
        int             getStatusCode();

        void            setPath(std::string& str);
        void            setErrorPath(std::string& str);

    private:
        int                                 _clientSocket;
        int                                 _method;
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
