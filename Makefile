# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/12 14:48:07 by mprofett          #+#    #+#              #
#    Updated: 2024/03/19 15:12:27 by nesdebie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ./webserv

SRC			= webserv.cpp \
			request_manager/srcs/Request.cpp \
			request_manager/srcs/RequestLine.cpp \
			request_manager/srcs/Cgi.cpp \
			response_manager/Response.cpp \
			response_manager/ErrorResponse.cpp \
			server/TcpListener.cpp \
			server/communications/run.cpp \
			server/utils/utils.cpp \
			server/configuration_file_parsing/directives.cpp \
			server/configuration_file_parsing/parser.cpp \
			server/configuration_file_parsing/tokenizer.cpp \
			server/configuration_file_parsing/utils.cpp \
			server/Server.cpp \
			server/Route.cpp \

OBJ			= ${SRC:.cpp=.o}

CC			= c++

RM			= rm -rf

CPPFLAGS	= -g -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g

all: ${NAME}

${NAME}: ${OBJ}
	${CC} ${CPPFLAGS} ${OBJ} -o ${NAME}

clean:
	${RM} ${OBJ}

fclean: clean
	${RM} ${NAME}

re:	fclean all

.PHONY:	all clean fclean re
