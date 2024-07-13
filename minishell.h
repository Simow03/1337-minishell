/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 16:02:34 by ayyassif          #+#    #+#             */
/*   Updated: 2024/07/13 16:43:00 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <stdlib.h>
# include <limits.h>
# include <fcntl.h>
# include <string.h>
# include <signal.h>

extern volatile sig_atomic_t	g_sigint_received;

//---------- STRUCTS ----------//
typedef enum e_quote
{
	NOT_Q,
	DOUBLE_Q,
	SINGLE_Q
}	t_quote;

typedef struct s_cmd
{
	char			*str;
	struct s_cmd	*next;
}	t_cmd;

typedef enum e_tree
{
	TR_COMMAND,
	TR_REDIR_IN,
	TR_HERE_DOC,
	TR_REDIR_OUT,
	TR_REDIR_APND,
	TR_PIPE
}	t_etree;

typedef struct s_tree
{
	int				node_type;
	void			*content;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

typedef enum e_token
{
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

typedef struct s_token
{
	char			*content;
	t_etoken		token_type;
	t_quote			quote;
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	char			*name;
	char			*value;
	char			**str_env;
	struct s_env	*next;
}	t_env;

typedef struct s_alloc
{
	void			*ptr;
	struct s_alloc	*next;
}	t_alloc;

//---------- PARSING ----------//
t_tree	*parsing(void);
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
int		cmd_size(t_token *token);
t_tree	*redir_tree(t_token *token);
t_token	*cmd_join_util(t_token **prev, t_token *token);
char	*merge_text(t_token **token, t_etoken token_type);
t_token	*cmd_handlers(t_token *token, t_token **prev);
void	free_tree(t_tree *tree);
t_token	*here_doc_handler(t_token *token);
t_token	*here_doc_expand(char *str, int is_quote);
char	*mergejoin(char *s1, char *s2);
int		global_return_int(int mode, int value);
t_token	*quote_expend(char *str, t_token *next, t_etoken token_type);
void	cmd_handler_util(t_token **token);
int		new_deli_size(t_token *token);
int		amb_error(t_token **prev, t_token *token, char *old_content);
void	error_hrdc(t_token *token, int pos);
t_token	*no_quote_expend(char *str, t_etoken token_type, t_token *next);
t_token	*heredoc_signal(t_token *token, char *deli, char *text, char *line);

//---------- LIBFT ----------//
void	ft_putstr_fd(char *s, int fd);
char	*ft_strjoin(char *s1, char *s2);
int		ft_isalpha(int c);
int		ft_strcmp(char *s1, char *s2);
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
int		ft_exit_atoi(char *str);
int		ft_isdigit(int c);
char	*ft_itoa(int n);
int		ft_dbl_strlen(char **str);
char	*ft_strcpy(char *dest, const char *src);
char	*ft_strcat(char *dest, const char *src);
int		ft_strchr(char *str, char c);

//---------- BUILTINS ----------//
void	echo(char **cmd);
void	env(t_env	*var);
int		builtin_exit(char **cmd);
void	pwd(void);
int		cd(char **cmd, t_env **myenv);
int		cd_dash_option(char **cmd, t_env **myenv, char *old_pwd);
int		get_home_dir(t_env **myenv, char *old_pwd);
void	cd_error(char *path);
int		check_old_path(char *old_path, char *old_pwd);
int		parse(char **cmd, t_env **myenv, char *old_pwd);
void	export(t_env **myenv, char **cmd);
char	*concat_value(char *s1, const char *s2);
int		split_cmd(t_env **myenv, char *s, t_env *input);
t_env	*sort_env(t_env *myenv);
int		is_valid_name(t_env **myenv, char *name, char *flag);
void	process_input(t_env **myenv, char *cmd);
void	add_var(char **env, t_env **myenv);
void	unset(t_env **myenv, char **cmd);
void	free_dbl_str(char **path);
void	free_env(t_env	*env);
void	free_input(t_env *input);

//---------- EXECUTION ----------//
void	execution(t_tree *tree, t_env **myenv, char **env);
void	ft_pipe(t_tree *tree, t_env **myenv, char **env);
void	left_redirect(t_tree *tree, t_env **myenv, char **env);
void	left_double_redirect(t_tree *tree, t_env **myenv, char **env);
void	right_redirect(t_tree *tree, t_env **myenv, char **env);
void	right_double_redirect(t_tree *tree, t_env **myenv, char **env);
void	check_args(char **cmd, t_env **myenv);
void	error_fork(void);
void	error_cmd(char *cmd);
void	error_path(char *cmd);
void	replace_last_cmd(char **cmd, t_env **myenv, char *flag);
void	create_str_env(t_env *myenv);
int		get_status(int status);
void	main_signo(int signo);
void	exec_signo(int signo);
void	signal_listener(void);

#endif