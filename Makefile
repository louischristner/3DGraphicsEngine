##
## EPITECH PROJECT, 2020
## 3DGraphicsEngine
## File description:
## Makefile
##

CC		=	g++

CFLAGS	=	-I./headers -g -Wall -Werror

NAME	=	graphicEngine

SRC		=	main.cpp				\
			src/Vect3D.cpp			\
			src/Mesh.cpp			\
			src/Matrix4x4.cpp		\
			src/Chunk.cpp			\
			src/GraphicEngine3D.cpp

DEPS	=	headers/Vect3D.hpp			\
			headers/Mesh.hpp			\
			headers/Matrix4x4.hpp		\
			headers/Chunk.hpp			\
			headers/GraphicEngine3D.hpp

OBJ		=	$(SRC:.cpp=.o)

FLAGS	=	-lsfml-graphics -lsfml-window -lsfml-system

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(FLAGS)

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
