NAME		= scop
OBJS_DIR	= Objs
SRCS_DIR	= Sources

FILES		= main inputs utils Camera Exception Face Material OpenGL_Manager Parser

SRCS		= $(addprefix $(SRCS_DIR)/, $(addsuffix .cpp, $(FILES)))
OBJS 		= $(addprefix $(OBJS_DIR)/, $(addsuffix .o, $(FILES)))

# ===---===---===---===---===---===---===---===---===---===---===---===---

ifeq ($(shell uname), Linux)
LINKS		= -L Libs `pkg-config --static --libs glfw3 glew` -lGL -lX11 -lpthread -lXrandr -lXi -ldl Libs/libSOIL.a
else
LINKS		=  -framework OpenGl -framework AppKit -framework IOkit Libs/mac/libglfw3.a Libs/mac/libGLEW.a Libs/mac/libSOIL.a
endif

# ===---===---===---===---===---===---===---===---===---===---===---===---

CC = clang++
CPPFLAGS = -Wall -Wextra -Werror -O3
SAN = -fsanitize=address -g
INCLUDES	= -I Includes -I glm

# ===---===---===---===---===---===---===---===---===---===---===---===---

all: $(OBJS_DIR) $(NAME)

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(NAME): $(OBJS)
	$(CC) $(CPPFLAGS) $(SAN) $(INCLUDES) $(OBJS) -o $(NAME) $(LINKS)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(SAN) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

run: all
	@./$(NAME)

rer: re
	@./$(NAME)

.PHONY: all clean fclean re run rer
