NAME		= scop
OBJS_DIR	= Objs
SRCS_DIR	= Sources

FILES		= main utils Exception Face OpenGL_Manager Parser

SRCS		= $(addprefix $(SRCS_DIR)/, $(addsuffix .cpp, $(FILES)))
OBJS 		= $(addprefix $(OBJS_DIR)/, $(addsuffix .o, $(FILES)))

# ===---===---===---===---===---===---===---===---===---===---===---===---

ifeq ($(shell uname), Linux)
# MLX = -lmlx_linux -Lmlx_linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz $(MLX_PATH)
LINKS		= -L Libs `pkg-config --static --libs glfw3 glew` -lGL -lX11 -lpthread -lXrandr -lXi -ldl Libs/libSOIL.a
else
LINKS		=  -framework OpenGl -framework AppKit -framework IOkit Libs/mac/libglfw3.a Libs/mac/libGLEW.a Libs/mac/libSOIL.a
# MLX = -lmlx -Lmlx -framework OpenGl -framework AppKit $(MLX_PATH)             #-framework Cocoa -framework CoreVideo
endif

# ===---===---===---===---===---===---===---===---===---===---===---===---

CC = clang++
CPPFLAGS = -Wall -Wextra -Werror -O3
SAN =# -fsanitize=address -g
INCLUDES	= -I Includes -I glm# `pkg-config --cflags glfw3`
# MATH = -lm

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

.PHONY: all clean fclean re
