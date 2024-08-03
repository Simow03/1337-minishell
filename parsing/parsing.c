/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 21:54:51 by ayyassif          #+#    #+#             */
/*   Updated: 2024/08/03 14:38:49 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*reading_line(void)
{
	char	*line;

	line = NULL;
	line = readline("minishell$ ");
	if (line == NULL)
	{
		printf("exit\n");
		exit(0);
	}
	if (line && *line)
	{
		add_history(line);
		return (line);
	}
	return (free(line), NULL);
}

void token_typer(t_token *token)
{
    while (token)
    {
        printf("%d\t\"%s\"\n", token->token_type, token->content);
        token = token->next;
    }
}

t_tree	*parsing(void)
{
	int			error;
	char		*line;
	char		*err_msg;
	t_token		*token;
	t_tree		*tree;

	line = reading_line();
	if (!line)
		return (NULL);
	error = 0;
	token = tokenizer(line, &error);
	token_typer(token);
	free(line);
	if (error)
		return (NULL);
	err_msg = syntax(token, &error);
	if (err_msg)
	{
		global_return_int(1, 258);
		error_hrdc(token, error);
		ft_putstr_fd(err_msg, STDERR_FILENO);
		free_token(token);
		return (free(err_msg), NULL);
	}
	tree = tree_planting(token);
	return (tree);
}
