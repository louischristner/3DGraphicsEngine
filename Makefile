##
## EPITECH PROJECT, 2020
## 3DGraphicsEngine
## File description:
## Makefile
##

CC		=	g++

NAME	=	graphicEngine

SRC		=	main.cpp				\
			src/Vect3D.cpp			\
			src/Mesh.cpp			\
			src/Matrix4x4.cpp		\
			src/GraphicEngine3D.cpp

OBJ		=	$(SRC:.cpp=.o)

FLAGS	=	-lsfml-graphics -lsfml-window -lsfml-system

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(FLAGS)

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
