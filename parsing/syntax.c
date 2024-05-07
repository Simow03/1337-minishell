/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 17:26:00 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/07 17:15:22 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//error types:
//0:	bash: syntax error near unexpected token '%s'
/*1:	bash: unexpected EOF while looking for matching `"'
		bash: syntax error: unexpected end of file	*/
//2:	bash: syntax error: unexpected end of file

static void	error_printer(int err_type, char *err_msg)
{
	if (err_type == 0)
		ft_putstr_fd("bash: syntax error near unexpected token `", STDERR_FILENO);
	if (err_type == 1)
		ft_putstr_fd("bash: unexpected EOF while looking for matching `"
			, STDERR_FILENO);
	if (err_type <= 1)
	{
		ft_putstr_fd(err_msg, STDERR_FILENO);
		ft_putstr_fd("\'\n", STDERR_FILENO);
	}
	if (err_type >= 1)
		ft_putstr_fd("bash: syntax error: unexpected end of file\n", STDERR_FILENO);
}

void	rdrctn_type(t_tokens *token)
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


static int	quote_checker(t_tokens *token)
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
				error_printer(1, text);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	syntax(t_tokens *token)
{
	int			frst_tm;

	frst_tm = 1;
	while (token)
	{
		if (token->token_type == 0 && quote_checker(token))
			return (1);
		if (token->token_type == 1)
		{
			if (!token->next)
				return (error_printer(0, "newline"), 1);
			else if (token->next->token_type >= 1)
				return (error_printer(0, token->next->token), 1);
		}
		if (token->token_type == 2 && !token->next)
			return (error_printer(2, token->token), 1);
		if (token->token_type == 2
			&& (frst_tm++ == 1 || token->next->token_type == 2))
			return (error_printer(0, token->token), 1);
		frst_tm++;
		token = token->next;
	}
	return (0);
}
