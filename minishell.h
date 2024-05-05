#ifndef MINISHELL_H
# define MINISHELL_H
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
} t_env;

//red_mod:    redirection type:
//0:    <
//1:    >
//2:    >>

typedef    struct s_files
{
        char            *file;
        int                red_mode;
        struct s_files    *next;
}    t_files;

typedef    struct s_cmd
{
    //command where it will be protoyped like this: ["command", "argument1", ... "argumentn", NULL].
    char            **cmd;
    //input string, if NULL read from standard input, 
    t_files            *input;
    //output string, if NULL write in standard output
    t_files            *output;
    struct s_cmd    *next;
}    t_cmd;

//------  LIBFT  -------//
void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
int		ft_strncmp(const char *str1, const char *str2, size_t size);
size_t	ft_strlen(const char *str);
char	**ft_split(char const *s, char c);
t_env	*ft_env_lstnew(char *name, char *value);
void	ft_envadd_back(t_env **var, t_env *new);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	**ft_envsplit(char const *s);
char	*ft_strdup(const char *str);
t_env	*ft_lstlast(t_env *env);
int		ft_atoi(const char *str);
int		ft_isdigit(int c);

#endif