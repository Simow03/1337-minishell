/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 16:02:34 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/14 01:31:32 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <limits.h>
# include <fcntl.h>
# include <string.h>

typedef	struct s_cmd
{
	char			*str;
	struct s_cmd	*next;
}	t_cmd;

typedef	struct s_tree
{
	//0:	command
	//1:	<
	//2:	<<
	//3:	>
	//4:	>>
	//5:	pipe
	int				node_type;
	//content:
	//char	**cmd;
	//char	*file; for input and output files
	//char	*here_doc, here_doc's input
	//NULL for pipe
	void			*content;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

//first token types:
//0:	text
//1:	rediraction
//2:	pipe

//second token types:
//0:	command
//1:	parameters
//2:	input rediraction <
//3:	input file
//4:	here_doc <<
//5:	delimeter
//6:	output rediraction >
//7:	output rediraction >>
//8:	output file
//9:	pipe

typedef struct s_tokens
{
	char			*token;
	int				token_type;
	int				is_quoted;
	struct s_tokens	*next;
}	t_tokens;

typedef struct s_env
{
	char			*name;
	char			*value;
	int				is_vis;
	struct s_env	*next;
}	t_env;

t_tree		*parsing(t_env *env);
t_tokens	*tokenizer(char *line, int *error);
char		*syntax(t_tokens *token, int *pos);
void		free_token(t_tokens *token);
t_tree		*tree_planting(t_tokens *token, t_env *env);
void		token_retyping(t_tokens *token);
void		free_tree(t_tree *tree);
char		*here_doc_handler(char	*delimeter, int is_quoted);
char		**malloc_cmd(t_tokens *token);
char		*expanding(char *token, t_env *env);
int			quote_checker(t_tokens *token, char	**err_msg);



void		ft_putstr_fd(char *s, int fd);
char		*ft_strjoin(char *s1, char *s2);
int			ft_isalpha(int c);
int			ft_strcmp(char *s1, char *s2);


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
char	*ft_itoa(int n);

void	free_env(t_env *env);

//---------- BUILTINS ----------//
void	echo(char **cmd);
void	env(t_env	*var);
int		builtin_exit(char **cmd, t_env **myenv);
void    pwd(void);
int		cd(char **cmd, t_env **env);
void	export(t_env **var, char **cmd);
void	add_var(char **env, t_env **var);


//---------- EXECUTION ----------//
void	execution(t_tree *tree, t_env **myenv, char **env);
void	ft_pipe(t_tree *tree, t_env **myenv, char **env);
void	left_redirect(t_tree *tree, t_env **myenv, char **env);
// int		left_double_redirect(t_tree *tree, t_env **myenv, char **env);
void	right_redirect(t_tree *tree, t_env **myenv, char **env);
// int		right_double_redirect(t_tree *tree, t_env **myenv, char **env);
void	check_args(char **cmd, t_env **myenv);
void	error_fork(void);
void	error_cmd(char *cmd);

#endif