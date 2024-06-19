/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 16:02:34 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/14 17:49:59 by ayyassif         ###   ########.fr       */
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

typedef enum e_quote
{
	NOT_Q,
	DOUBLE_Q,
	SINGLE_Q
}	t_quote;

typedef	struct s_cmd
{
	char			*str;
	struct s_cmd	*next;
}	t_cmd;

typedef enum e_tree {
	TR_COMMAND,
	TR_REDIR_IN,
	TR_HERE_DOC,
	TR_REDIR_OUT,
	TR_REDIR_APND,
	TR_PIPE
}	t_etree;

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

typedef enum e_token {
	TK_COMMAND, //0
	TK_SPACE, //1
	TK_REDIR_IN, //2
	TK_REDIR_OUT, //3
	TK_REDIR_APND, //4
	TK_REDIR_FILE, //5
	TK_HERE_DOC, //6
	TK_DELIMETER, //7
	TK_PIPE //8
}	t_etoken;

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
	char			*content;
	t_etoken		token_type;
	t_quote			quote;
	struct s_tokens	*next;
}	t_token;

typedef struct s_env
{
	char			*name;
	char			*value;
	int				is_hidden;
	struct s_env	*next;
}	t_env;

t_tree	*parsing();
t_token	*tokenizer(char *line, int *error);
char	*syntax(t_token *token, int *pos);
void	free_token(t_token *token);
t_tree	*tree_planting(t_token *token);
// void		token_retyping(t_token *token);
// void		free_tree(t_tree *tree);
char	*here_doc_handler(t_token *token, t_env *env);
int		get_next_expand(char *text, char *result, int *i);
//char		**malloc_cmd(t_token *token);
char	*expanding(char *text, int type);
//int			quote_checker(t_token *token, char	**err_msg);
char	*error_printer(int err_type, char *err_msg);
//void		hrdc_tree(t_tree *new, t_token *token);
t_env	*global_env(t_env *env, int mode);
int		ambiguous(char *text, char quote, int is_ambiguous);
int		get_next_expand(char *text, char *result, int *i);
int		sizeofexpndng(char *text);
//char	*value_fetcher(char *text, t_env *env, int *size);
int		global_return_int(int mode, int value);
char	*global_return_str(int mode, int value);

char	*ft_strjoin(char *s1, char *s2);
int		ft_isalpha(int c);
int		ft_strcmp(char *s1, char *s2);
char	*ft_itoa(int n);


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

#endif