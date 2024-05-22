/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 17:26:00 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/21 10:29:56 by ayyassif         ###   ########.fr       */
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

static void	rdrctn_type(t_tokens *token)
{
	if (token->token[1] == '<')
	{
		token->token_type = 4;
		token->next->token_type = 5;
	}
	else if (token->token[0] == '<')
	{
		token->token_type = 2;
		token->next->token_type = 3;
	}
	if (token->token[1] == '>')
	{
		token->token_type = 7;
		token->next->token_type = 8;
	}
	else if (token->token[0] == '>')
	{
		token->token_type = 6;
		token->next->token_type = 8;
	}
}

void	token_retyping(t_tokens *token)
{
	int	is_cmd;

	is_cmd = 1;
	while (token)
	{
		if (token->token_type == 0 && is_cmd)
			is_cmd = 0;
		else if (token->token_type == 0 && !is_cmd)
			token->token_type = 1;
		else if (token->token_type == 1)
		{
			rdrctn_type(token);
			token = token->next;
		}
		else if (token->token_type == 2)
		{
			token->token_type = 9;
			is_cmd = 1;
		}
		token = token->next;
	}
}

int	quote_checker(t_tokens *token, char	**err_msg)
{
	char	quote;
	char	*text;
	int		i;

	text = token->token;
	i = 0;
	while (text[i])
	{
		if (text[i] == '\'' || text[i] == '\"')
		{
			token->is_quoted = 1;
			quote = text[i++];
			while (text[i] && text[i] != quote)
				i++;
			if (!text[i])
			{
				*err_msg = error_printer(1, text);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

char	*syntax(t_tokens *token, int *pos)
{
	char	*err_msg;

	*pos = 1;
	while (token)
	{
		if (token->token_type == 0 && quote_checker(token, &err_msg))
			return (err_msg);
		if (token->token_type == 1)
		{
			if (!token->next)
				return (error_printer(0, "newline"));
			else if (token->next->token_type >= 1)
				return (error_printer(0, token->next->token));
		}
		if (token->token_type == 2 && !token->next)
			return (error_printer(2, token->token));
		if (token->token_type == 2
			&& (*pos == 1 || token->next->token_type == 2))
			return (error_printer(0, token->token));
		(*pos)++;
		token = token->next;
	}
	*pos = 0;
	return (NULL);
}
