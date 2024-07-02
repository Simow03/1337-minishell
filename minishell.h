/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 16:02:34 by ayyassif          #+#    #+#             */
/*   Updated: 2024/07/02 15:52:31 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/ioctl.h>
# include <stdlib.h>
# include <limits.h>
# include <fcntl.h>
# include <string.h>
# include <sys/stat.h>
# include <signal.h>

extern volatile sig_atomic_t sigint_received;

//---------- STRUCTS ----------//
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
	int				is_hidden;
	char			**str_env;
	struct s_env	*next;
}	t_env;




//---------- PARSING ----------//
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
t_token	*cmd_join_util(t_token **prev, t_token *token);
char	*merge_text(t_token **token, t_etoken token_type);
t_token	*cmd_handlers(t_token *token, t_token **prev);
void	free_tree(t_tree *tree);
t_token	*here_doc_handler(t_token *token);
t_token	*here_doc_expand(char *str, int is_quote);
char	*mergejoin(char *s1, char *s2);

t_env		*create_env(char **env);
void		free_env(t_env	*env);



//---------- LIBFT ----------//
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
int		ft_exit_atoi(char *str);
int		ft_isdigit(int c);
char	*ft_itoa(int n);
int		ft_dbl_strlen(char **str);
char	*ft_strcpy(char *dest, const char *src);
char	*ft_strcat(char *dest, const char *src);


//---------- BUILTINS ----------//
void	echo(char **cmd);
void	env(t_env	*var);
int		builtin_exit(char **cmd, t_env **myenv);
void    pwd(void);
int		cd(char **cmd, t_env **myenv);
void	cd_dash_option(char **cmd, t_env **myenv, char *old_pwd);
void	get_home_dir(t_env **myenv, char *old_pwd);
void	cd_error(char *path);
void	export(t_env **myenv, char **cmd);
int		is_valid_name(t_env **myenv, char *name, char *flag);
void	process_input(t_env **myenv, char *cmd);
void	add_var(char **env, t_env **myenv);
void	unset(t_env **myenv, char **cmd);
void	free_env(t_env	*env);
void	decrement_shlvl(t_env **myenv);



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



void	sigint_handler(int signo);
int		global_return_int(int mode, int value);
void	signal_listener(void);
char	*init_prompt(void);


extern volatile sig_atomic_t sigint_received;

#endif