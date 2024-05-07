/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 16:02:34 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/07 15:39:49 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <fcntl.h>

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
	struct s_env	*next;
}	t_env;

t_tree		*parsing(void);
t_tokens	*tokenizer(char *line, int *error);
int			syntax(t_tokens *token);
void		free_token(t_tokens *token);
t_tree		*tree_planting(t_tokens *token);
void		token_retyping(t_tokens *token);
void		free_tree(t_tree *tree);
char		*here_doc_handler(char	*delimeter);
char		**malloc_cmd(t_tokens *token);

void		ft_putstr_fd(char *s, int fd);
size_t		ft_strlen(char *s);
int			ft_strncmp(char *s1, char *s2, size_t n);
char		*ft_strdup(char *s1);

#endif