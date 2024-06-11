/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:48:12 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/02 11:22:18 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*error_printer(int err_type, char *err_msg)
{
	char	*str;
	char	*tmp;

	if (err_type == 0)
	{
		str = ft_strjoin("bash: syntax error near unexpected token `", err_msg);
		tmp = ft_strjoin(str, "\'\n");
	}
	if (err_type == 1)
	{
		str = ft_strjoin("bash: unexpected EOF while looking for matching `", err_msg);
		tmp = ft_strjoin(str, "\'\nbash: syntax error: unexpected end of file\n");
	}
	if (err_type == 2)
		return (ft_strdup("bash: syntax error: unexpected end of file\n"));
	if (err_type == 3)
	{
		str = ft_strjoin("bash: ", err_msg);
		tmp = ft_strjoin(str, ": ambiguous redirect\n");
	}
	return (free(str), tmp);
}

void	syntax_checker(t_token *token, int *pos, t_token *next, char *err_msg)
{
	int	len;

	if (token->token_type == TK_REDIR_IN || token->token_type == TK_REDIR_OUT
		|| token->token_type == TK_REDIR_APND || token->token_type == TK_HERE_DOC)
	{
		if (!next)
			err_msg = error_printer(0, "newline");
		if (next->token_type != TK_REDIR_FILE || next->token_type != TK_DELIMETER)
			err_msg = error_printer(0, next->content);
	}
	if (token->token_type == TK_PIPE && !next)
		err_msg = error_printer(2, token->content);
	if (token->token_type == TK_PIPE
		&& (*pos == 1 || next->token_type == TK_PIPE))
		err_msg = error_printer(0, token->content);
	if (token->token_type == TK_COMMAND || token->token_type == TK_REDIR_FILE
		|| token->token_type == TK_DELIMETER && token->quote != NOT_Q)
	{
		len = ft_strlen(token->content);
		if (token->quote == DOUBLE_Q && token->content[len - 1] != '\"')
			*err_msg = error_printer(1, "\"");
		if (token->quote == SINGLE_Q && token->content[len - 1] != '\'')
			*err_msg = error_printer(1, "\'");
	}	
}

char	*syntax2(t_token *token, int *pos)
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
		syntax_checker(token, pos, next, err_msg);
		if (err_msg)
			return (err_msg);
		(*pos)++;
		token = next;
	}
}
