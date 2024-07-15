NAME = minishell

CC = cc

RLP=$(shell brew --prefix readline)

LDFLAGS= -L$(RLP)/lib

CIFLAGS= -I$(RLP)/include

CFLAGS = $(CIFLAGS) -Wall -Wextra -Werror -g -fsanitize=address

FILES = main.c builtins/cd.c builtins/cd_utils.c builtins/echo.c builtins/env.c builtins/exit.c \
	builtins/export.c builtins/export_utils.c builtins/pwd.c builtins/unset.c execution/execution.c \
	execution/ft_pipe.c execution/redirections.c execution/utils.c execution/error_handle.c execution/signals.c \
	libft/ft_atoi.c libft/ft_dbl_strlen.c libft/ft_env_lstnew.c libft/ft_envadd_back.c libft/ft_envsplit.c \
	libft/ft_isdigit.c libft/ft_itoa.c libft/ft_lstlast.c libft/ft_putchar_fd.c libft/ft_putstr_fd.c libft/ft_split.c \
	libft/ft_strdup.c libft/ft_strlen.c libft/ft_strncmp.c libft/ft_substr.c libft/libft1.c parsing/cmd_join.c parsing/cmd.c \
	parsing/expanding.c parsing/here_doc.c parsing/syntax.c parsing/tokenizer.c parsing/tree.c parsing/tree_utils.c \
	parsing/utils.c parsing/parsing.c libft/ft_strcpy.c libft/ft_strcat.c libft/ft_strchr.c execution/str_env.c parsing/error_free.c \
	builtins/cd_errors.c builtins/export_sorting.c builtins/free_utils.c

HEADER = minishell.h

OBJS = $(FILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME) -lreadline

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJS_BONUS)
	$(CC) $(CFLAGS) $(OBJS_BONUS) -o $(NAME_BONUS)

bonus/%_bonus.o: bonus/%_bonus.c $(HEADER_BONUS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
	rm -f $(NAME) $(NAME_BONUS)

re: fclean all

.phony: clean