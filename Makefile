# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: achansar <achansar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/12 14:48:07 by mprofett          #+#    #+#              #
#    Updated: 2024/03/01 10:54:38 by achansar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ./webserv

SRC			= webserv.cpp \
			request_manager/srcs/Request.cpp \
			request_manager/srcs/RequestLine.cpp \
			response_manager/Response.cpp \
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

CPPFLAGS	= -g -Wall -Wextra -Werror -std=c++98

all: ${NAME}

${NAME}: ${OBJ}
	${CC} ${CPPFLAGS} ${OBJ} -o ${NAME}

clean:
	${RM} ${OBJ}

fclean: clean
	${RM} ${NAME}

re:	fclean all

.PHONY:	all clean fclean re
