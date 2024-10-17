SRCS_DIR	=	srcs/

SRC			=	main.c\
				handle.c\
				handle32.c\
				handle64.c\
				print.c\
				sort.c\
				utils.c\

SRCS 		=	$(addprefix $(SRCS_DIR), $(SRC))

OBJS_DIR	=	objs/

OBJ			=	$(SRC:.c=.o)

OBJS		=	$(addprefix $(OBJS_DIR), $(OBJ))

INCS		=	-I includes/

GCC			=	gcc -Wall -Wextra -Werror -g

NAME		=	ft_nm

all			:	$(NAME)

bonus		:	$(NAME)

$(NAME)		:	$(OBJS_DIR) $(OBJS)
					$(GCC) $(OBJS) -o $(NAME) $(INCS)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
				 	$(GCC) -c $< -o $@ $(INCS)

$(OBJS_DIR)	:
					mkdir $(OBJS_DIR)

clean		:
					rm -rf $(OBJS_DIR)

fclean		:	clean
					rm -f $(NAME)

re			:	fclean all

.PHONY		:	all bonus clean fclean re

# SRCS_DIR	=	srcs/
# SRC			=	main.c
# SRCS		=	$(addprefix $(SRCS_DIR)/, $(SRC))

# OBJS_DIR	=	objs/
# OBJ			=	$(SRC:.c=.o)
# OBJS		=	$(addprefix $(OBJS_DIR)/, $(OBJ))

# GCC			=	gcc -Wall -Wextra -Werror
# NAME		=	ft_nm

# all			:	$(OBJS_DIR) $(NAME)

# $(NAME)		:	$(OBJS)
# 				$(GCC) -o $(NAME) $(OBJS)

# $(OBJS_DIR)%.o: $(SRCS_DIR)%.c | $(OBJS_DIR)
# 				$(GCC) -c -o $@ $<

# $(OBJS_DIR):
# 				mkdir -p $(OBJS_DIR)

# clean		:
# 				rm -rf $(OBJS_DIR)

# fclean		:	clean
# 				rm -f $(NAME)

# re			:	fclean all

# .PHONY		:	all clean fclean re
