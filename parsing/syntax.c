/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 17:26:00 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/30 11:57:14 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//error types:
//0:	bash: syntax error near unexpected token '%s'
/*1:	bash: unexpected EOF while looking for matching `"'
		bash: syntax error: unexpected end of file	*/
//2:	bash: syntax error: unexpected end of file
//3:	bash: %s: ambiguous redirect

char	*error_printer(int err_type, char *err_msg)
{
	char	*str;
	char	*tmp;

	if (err_type == 0)
	{
		str = ft_strjoin("minishell: syntax error near unexpected token `", err_msg);
		tmp = ft_strjoin(str, "\'\n");
	}
	if (err_type == 1)
	{
		str = ft_strjoin("minishell: unexpected EOF while looking for matching `", err_msg);
		tmp = ft_strjoin(str, "\'\nminishell: syntax error: unexpected end of file\n");
	}
	if (err_type == 2)
		return (ft_strdup("minishell: syntax error: unexpected end of file\n"));
	if (err_type == 3)
	{
		str = ft_strjoin("minishell: ", err_msg);
		tmp = ft_strjoin(str, ": ambiguous redirect\n");
	}
	return (free(str), tmp);
}

static void	syntax_checker(t_token *token, int *pos, t_token *next, char **err_msg)
{
	char	quote[2];
	int		len;

	quote[1] = '\0';
	if (token->token_type == TK_REDIR_IN || token->token_type == TK_REDIR_OUT
		|| token->token_type == TK_REDIR_APND || token->token_type == TK_HERE_DOC)
	{
		if (!next)
			*err_msg = error_printer(0, "newline");
		else if (next->token_type != TK_REDIR_FILE && next->token_type != TK_DELIMETER)
			*err_msg = error_printer(0, next->content);
	}
	else if (token->token_type == TK_PIPE && !next)
		*err_msg = error_printer(2, token->content);
	else if (token->token_type == TK_PIPE
		&& (*pos == 1 || next->token_type == TK_PIPE))
		*err_msg = error_printer(0, token->content);
	else if ((token->token_type == TK_COMMAND || token->token_type == TK_REDIR_FILE
		|| token->token_type == TK_DELIMETER) && token->quote != NOT_Q)
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
		if (next && (token->token_type == TK_DELIMETER || token->token_type == TK_REDIR_FILE) && next->token_type == TK_COMMAND)
			next->token_type = token->token_type;
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
