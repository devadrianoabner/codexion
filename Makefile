# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adrianda <adrianda@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/11/13 17:41:51 by adrianda          #+#    #+#              #
#    Updated: 2026/11/13 17:41:52 by adrianda         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= codexion

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread
INCLUDES	= -Iincludes

SRC_DIR		= src
OBJ_DIR		= obj

SRCS		= $(SRC_DIR)/main.c \
			  $(SRC_DIR)/init.c \
			  $(SRC_DIR)/utils.c \
			  $(SRC_DIR)/queue.c \
			  $(SRC_DIR)/stop.c \
			  $(SRC_DIR)/monitor.c \
			  $(SRC_DIR)/dongles.c \
			  $(SRC_DIR)/release.c \
			  $(SRC_DIR)/coder.c

OBJS		= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c includes/codexion.h
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re