/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 17:26:00 by ayyassif          #+#    #+#             */
/*   Updated: 2024/07/13 16:32:11 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	syntax_conditions(t_token *next, t_token *token)
{
	if (token->token_type == TK_REDIR_IN || token->token_type == TK_REDIR_OUT
		|| token->token_type == TK_REDIR_APND
		|| token->token_type == TK_HERE_DOC)
		return (1);
	else if (next->token_type != TK_REDIR_FILE
		&& next->token_type != TK_DELIMETER)
		return (2);
	else if ((token->token_type == TK_COMMAND
			|| token->token_type == TK_REDIR_FILE
			|| token->token_type == TK_DELIMETER) && token->quote != NOT_Q)
		return (3);
	return (0);
}

static void	syntax_checker(t_token *token, int *pos,
	t_token *next, char **err_msg)
{
	char	quote[2];
	int		len;

	quote[1] = '\0';
	if (syntax_conditions(next, token) == 1)
	{
		if (!next)
			*err_msg = error_printer(0, "newline");
		else if (syntax_conditions(next, token) == 2)
			*err_msg = error_printer(0, next->content);
	}
	else if (token->token_type == TK_PIPE && !next)
		*err_msg = error_printer(2, token->content);
	else if (token->token_type == TK_PIPE
		&& (*pos == 1 || next->token_type == TK_PIPE))
		*err_msg = error_printer(0, token->content);
	else if (syntax_conditions(next, token) == 3)
	{
		quote[0] = token->content[0];
		len = ft_strlen(token->content);
		if (len == 1 || token->content[len - 1] != token->content[0])
			*err_msg = error_printer(1, quote);
	}
}

char	*syntax(t_token *token, int *pos)
{
	t_token	*next;
	char	*err_msg;

	*pos = 1;
	err_msg = NULL;
	while (token)
	{
		next = token->next;
		if (next && next->token_type == TK_SPACE)
			next = next->next;
		syntax_checker(token, pos, next, &err_msg);
		if (err_msg)
			return (err_msg);
		(*pos)++;
		token = next;
	}
	return (NULL);
}
