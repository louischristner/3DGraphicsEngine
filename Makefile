##
## EPITECH PROJECT, 2020
## 3DGraphicsEngine
## File description:
## Makefile
##

CC		=	g++

NAME	=	bin

SRC		=	main.cpp

OBJ		=	$(SRC:.cpp=.o)

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
