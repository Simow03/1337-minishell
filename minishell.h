/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 16:02:34 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/12 15:11:35 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef P_MINISHELL_H
# define P_MINISHELL_H

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
	int				is_hidden;
	struct s_env	*next;
}	t_env;

t_tree		*parsing(t_env *env);
t_tokens	*tokenizer(char *line, int *error);
char		*syntax(t_tokens *token, int *pos);
void		free_token(t_tokens *token);
t_tree		*tree_planting(t_tokens *token, t_env *env);
void		token_retyping(t_tokens *token);
void		free_tree(t_tree *tree);
char		*here_doc_handler(char	*delimeter, t_env *env, int is_quoted);
char		*value_fetcher(char *token, t_env *env, int *size);
int			get_next_expand(char *text, t_env *env, char *result, int *i);
char		**malloc_cmd(t_tokens *token);
char		*expanding(char *token, t_env *env);
int			quote_checker(t_tokens *token, char	**err_msg);



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



t_env		*create_env(char **env);
void		free_env(t_env	*env);

void	echo(char **cmd);
void	env(t_env	*var);
int	builtin_exit(char **cmd, char *prompt);
void    pwd(void);
void	add_var(char **env, t_env **var);

#endif