 # NAME ===============================================================
NAME =		ircserv
# ====================================================================

# FILES ==============================================================
FILES =		main.cpp							\
			server/server.cpp					\
			server/socketManager.cpp			\
			channel/Channel.cpp					\
			channel_aspects/ChannelAspects.cpp	\
			user/User.cpp						\
			cmd/cmd.cpp							\
# ====================================================================

# HEADER =============================================================
HEADER =	server/server.hpp					\
			channel/Channel.hpp					\
			channel_aspects/ChannelAspects.hpp	\
			user/User.hpp						\
			cmd/cmd.hpp							\

# ====================================================================

# FLAGS ==============================================================
FLAGS =		-Wall -Wextra -Werror -std=c++98
# ====================================================================

# COMPIL =============================================================
CC =		c++
# ====================================================================

# SUB_DIR_LST ========================================================
SUB_DIR_LST =	server channel user cmd channel_aspects
# ====================================================================

# OBJS_DIR ===========================================================
OBJS_DIR =	.objs
# ====================================================================

# OBJ ================================================================
OBJ = $(addprefix $(OBJS_DIR)/, $(FILES:.cpp=.o))
# ====================================================================

# SUB_DIR ============================================================
SUB_DIR		=	$(addprefix $(OBJS_DIR)/,$(SUB_DIR_LST))
# ====================================================================

all:				$(NAME)

$(NAME):			$(OBJ)
					$(CC) $(OBJ) -o $(NAME)

$(OBJS_DIR)/%.o:	%.cpp Makefile $(HEADER) | $(OBJS_DIR)
					${CC} ${FLAGS} -c $< -o $@

$(OBJS_DIR):
					mkdir -p $(OBJS_DIR)
					mkdir -p $(SUB_DIR)

clean :
					rm -f ${OBJ} ${OBJ_B} $(OBJS_DIR)/*.o 

fclean :
					rm -f ${OBJ} ${OBJ_B} $(OBJS_DIR)/*.o
					rm -f ${NAME}
					rm -rf $(OBJS_DIR)

re :				fclean all

.PHONY:				all clean fclean re

# ====================================================================