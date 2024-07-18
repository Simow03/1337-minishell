/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 17:10:51 by ayyassif          #+#    #+#             */
/*   Updated: 2024/07/18 14:03:11 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*no_quote_expend(char *str, t_etoken token_type, t_token *next)
{
	t_token	*new;
	int		size;

	new = (t_token *)malloc(sizeof(t_token));
	new->quote = NOT_Q;
	new->token_type = token_type;
	new->next = next;
	new->content = NULL;
	size = 0;
	if (str)
	{
		while (str[size] && str[size] != ' ')
			size++;
		if (*str == ' ')
		{
			new->token_type = TK_SPACE;
			while (str && *str == ' ')
				str++;
		}
		else
			new->content = ft_substr(str, 0, size);
		if (str[size])
			new->next = no_quote_expend(str + size, token_type, next);
	}
	return (new);
}

t_token	*cmd_handlers(t_token *token, t_token **prev)
{
	t_token	*t_tmp;
	char	*s_tmp;

	t_tmp = token;
	s_tmp = token->content;
	if (token->token_type == TK_HERE_DOC)
		token->next = here_doc_handler(token->next);
	if (token->token_type == TK_HERE_DOC && !token->next)
		return (NULL);
	if (token->token_type == TK_COMMAND || token->token_type == TK_REDIR_FILE)
	{
		cmd_handler_util(&token);
		if (s_tmp[0] == '$' || t_tmp->quote != NOT_Q)
			free(s_tmp);
		if (t_tmp && t_tmp->quote != SINGLE_Q && token != t_tmp)
			free(t_tmp);
		if (*prev)
			(*prev)->next = token;
	}
	return (token);
}

t_token	*cmd_join_util(t_token **prev, t_token *token)
{
	static char	*old_content;

	if (token->token_type == TK_REDIR_FILE
		&& *prev && (*prev)->token_type != TK_REDIR_FILE)
	{
		free(old_content);
		old_content = old_str(token);
	}
	token = cmd_handlers(token, prev);
	if (!token || (token->token_type == TK_REDIR_FILE
			&& amb_error(prev, token, old_content)))
		return (NULL);
	if (token && !token->next)
	{
		free(old_content);
		old_content = NULL;
	}
	return (token);
}

t_token	*cmd_join(t_token *token)
{
	t_token	*prev;
	t_token	*start;

	prev = NULL;
	start = token;
	while (token)
	{
		token = cmd_join_util(&prev, token);
		if (!token)
		{
			free_token(start);
			return (NULL);
		}
		if (!prev)
			start = token;
		prev = token;
		while (token && token->next && token->next->quote == DOUBLE_Q
			&& (!token->next->content || token->next->content[0] != '\"'))
			token = token ->next;
		if (token)
			token = token->next;
	}
	return (start);
}
