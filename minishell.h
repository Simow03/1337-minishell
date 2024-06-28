/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 16:02:34 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/28 10:04:38 by ayyassif         ###   ########.fr       */
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
	int				node_type;
	void			*content;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

typedef enum e_token {
	TK_COMMAND,
	TK_SPACE,
	TK_REDIR_IN,
	TK_REDIR_OUT,
	TK_REDIR_APND,
	TK_REDIR_FILE,
	TK_HERE_DOC,
	TK_DELIMETER,
	TK_HRDC_CONTENT,
	TK_PIPE
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
int		get_next_expand(char *text, char *result, int *i);
char	*error_printer(int err_type, char *err_msg);
t_env	*global_env(t_env *env, int mode);
int		get_next_expand(char *text, char *result, int *i);
int		global_return_int(int mode, int value);
char	*global_return_str(int mode, int value);
char	*value_fetcher(char *text, int *size);
t_tree	*tree_branches(t_token *token);
char	*old_str(t_token *token);
int		has_content(t_token *token);
t_token	*quote_expend(char *str, t_token *next, t_etoken token_type);
t_token	*cmd_join(t_token *token);
t_tree	*cmd_tree(char	**cmd);
void	error_hrdc(t_token *token, int pos);
int		cmd_size(t_token *token);
t_tree	*redir_tree(t_token *token);
t_tree	*hrdc_tree(t_token *token);
t_token	*cmd_join_util(t_token **prev, t_token *token);
char	*merge_text(t_token **token, t_etoken token_type);
t_token	*cmd_handlers(t_token *token, t_token **prev);
void	free_tree(t_tree *tree);
t_token	*here_doc_handler(t_token *token);
t_token	*here_doc_expand(char *str);


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