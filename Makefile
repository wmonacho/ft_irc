 # NAME ===============================================================
NAME =		ircserv
# ====================================================================

# FILES ==============================================================
FILES =		main.cpp							\
			server/server.cpp					\
			server/socketManager.cpp			\
			channel/Channel.cpp					\
			user_aspects/UserAspects.cpp		\
			user/User.cpp						\
			cmd/cmd.cpp							\
			cmd/join.cpp						\
			cmd/privmsg.cpp						\
			cmd/mode.cpp						\
			cmd/kick.cpp						\
			cmd/list.cpp						\
			cmd/invite.cpp						\
			cmd/names.cpp						\
			cmd/topic.cpp						\
			cmd/part.cpp						\
			cmd/quit.cpp
# ====================================================================

# HEADER =============================================================
HEADER =	server/server.hpp					\
			channel/Channel.hpp					\
			user_aspects/UserAspects.hpp	\
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
SUB_DIR_LST =	server channel user cmd user_aspects
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