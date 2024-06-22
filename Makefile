NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address

# FILES = parsing/tree.c parsing/tokenizer.c parsing/utils.c main.c libft/libft1.c\
# 	parsing/syntax.c parsing/parsing.c parsing/here_doc.c extra/env.c parsing/expanding.c
FILES = extra/env.c libft/ft_atoi.c libft/ft_env_lstnew.c libft/ft_envadd_back.c\
	libft/ft_envsplit.c libft/ft_isdigit.c libft/ft_lstlast.c libft/ft_putchar_fd.c\
	libft/ft_putstr_fd.c libft/ft_split.c libft/ft_strdup.c libft/ft_strlen.c\
	libft/ft_strncmp.c libft/ft_substr.c libft/libft1.c main.c\
	parsing/here_doc.c parsing/parsing.c parsing/syntax.c parsing/tokenizer.c\
	parsing/utils.c parsing/tree.c parsing/cmd.c parsing/expanding.c parsing/tree_utils.c
#	parsing/expanding2.c parsing/expanding.c parsing/tree.c 

HEADER = minishell.h

OBJS = $(FILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lreadline

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.phony: clean all fclean re