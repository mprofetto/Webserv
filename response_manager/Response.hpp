/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/04/18 13:29:37 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <map>
#include "../request_manager/includes/Request.hpp"
#include "../request_manager/includes/Cgi.hpp"
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
        Response(const Response& src);
        Response& operator=(const Response& src);
        ~Response();

    // MEMBER FUNCTIONS
        void            getBody(bool autodindex, Route *route);
        void            buildResponse(Route* route);
        void            buildErrorResponse();
        std::string     getHeaders(const int s);
        std::string     getReason(int sc);
        int             fileTransfer();
        int             isRedirect();
        void            redirectClient();
        void            sendFile();
        int             receiveFile();
        int             deleteFile();
        std::string     extractFileBody(std::string request);
        int             generateAutoindex();
        int             handlePostRequest();
        int             handleForm();

    // UTILS
        void	        getFullPath(Route *route, std::string uri);
        std::string     extractExtension(std::string uri);
        std::string     extractFileName();
        std::string     getMimeType();
        bool            isDirectory(std::string path);

    // GET & SET
        unsigned long   getBytesSend() const;
        std::string     getResponse() const;
        std::string     getPath() const;
        int             getStatusCode() const;
        int             getClientSocket() const;

        void            setPath(std::string& str);
        void            setErrorPath(std::string& str);
        void            setBody(std::string& str);
        void            setHeaders(std::string& str);

        void            addToBytesSend(unsigned long bytes_to_add);

    private:
        bool                                _cgi;
        int                                 _clientSocket;
        unsigned long                       _bytesSend;
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
