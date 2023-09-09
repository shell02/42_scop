NAME = scop
CC = c++
INCLUDE = ./include
LIB = ./lib -lglfw3
CFLAGS = -Wall -Werror -Wextra
SRCS_DIR = ./src
OBJS_DIR = ./objs
SRCS = main.cpp Vector.cpp Matrix.cpp log.cpp utils.cpp Shader.cpp Camera.cpp Texture.cpp Light.cpp Mesh.cpp Model.cpp MTL.cpp
OBJS := $(patsubst %.cpp, $(OBJS_DIR)/%.o, $(SRCS))

all: mkObjDir $(NAME)

mkObjDir:
	@mkdir -p $(OBJS_DIR)

$(NAME): $(OBJS)
	@printf "\e[0;32mDone compiling objects!            \n\e[0m"
	@$(CC) $(CFLAGS) $(OBJS) ./src/glad.c -o $(NAME) -I$(INCLUDE) -L$(LIB)
	@echo "\e[0;32mProgram" $(NAME) "successfully created!\e[0m"

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE) -L$(LIB)
	@printf "\e[0;32mCompiling $@...        \e[0m\r"

clean:
	@rm -rf $(OBJS_DIR)
	@echo "\e[1;33mObjects files removed!\e[0m"

fclean: clean
	@rm -rf $(NAME)
	@echo "\e[1;33mProgram file removed!\e[0m"
	@rm -rf gl.log

re: fclean all

.PHONY: all clean fclean re