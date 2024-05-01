#ifndef MINISHELL_H
# define MINISHELL_H
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

typedef    struct s_here_doc
{
	//pipe's file descriptor 
	int        fd;
	char    *delimeter;
	//checks if the delimeter is quoted, if so the input should not be expanded.
	int        deli_quoted;
}    t_here_doc;

typedef    struct s_cmd
{
	//command where it will be protoyped like this: ["command", "argument1", ... "argumentn", NULL].
	char            **cmd; 
	//input string, if NULL read from standard input, if "here_doc" read from the here doc.
	char            input;
	//info about the here_doc
	t_here_doc        *here_doc;
	//output string, if NULL write in standard output
	char            *output;
	//redirection type, if 0 it means ">", if 1 it means ">>".
	int                red_mode;
	struct s_cmd    *next_cmd;
}    t_cmd;


//------  LIBFT  -------//
void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
int		ft_strncmp(const char *str1, const char *str2, size_t size);
size_t	ft_strlen(const char *str);

#endif