/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 17:10:51 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/29 15:38:51 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*quote_expend(char *str, t_token *next, t_etoken token_type)
{
	t_token	*new;
	int		size;

	size = 0;
	if (str[0] == '\"')
		return (next);
	new = (t_token *)malloc(sizeof(t_token));
	new->token_type = token_type;
	new->quote = DOUBLE_Q;
	while (str[size] && str[size] != '\"' && str[size] != '$')
		size++;
	if (str[0] == '$')
		new->content = ft_strdup(value_fetcher(++str, &size));
	else
		new->content = ft_substr(str, 0, size);
	new->next = quote_expend(str + size, next, token_type);
	return (new);
}

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

int	amb_error(t_token **prev, t_token *token, char *old_content)
{
	char		*err_msg;
	static int	is_full;

	is_full = 1;
	if (*prev && (*prev)->token_type != TK_REDIR_FILE)
		is_full = 0;
	while (token && token->token_type == TK_REDIR_FILE)
	{
		if (token->quote != NOT_Q || (token->content
			&& (token->content[0] != '$' || value_fetcher(token->content + 1, NULL))))
			is_full = 1;
		token = token->next;
	}
	if (token && token->token_type == TK_SPACE && token->next
		&& token->next->token_type == TK_REDIR_FILE)
		is_full = 0;
	if (is_full)
		return (0);
	err_msg = error_printer(3, old_content);
	ft_putstr_fd(err_msg, STDERR_FILENO);
	free(err_msg);
	return (1);
}

t_token	*cmd_handlers(t_token *token, t_token **prev)
{
	t_token	*t_tmp;
	char	*s_tmp;

	t_tmp = token;
	s_tmp = token->content;
	if (token->token_type == TK_HERE_DOC)
		token->next = here_doc_handler(token->next);
	if (token->token_type == TK_COMMAND || token->token_type == TK_REDIR_FILE)
	{
		if (token->quote == NOT_Q && token->content && token->content[0] == '$')
			token = no_quote_expend(value_fetcher(token->content + 1, NULL),
				token->token_type, token->next);
		else if (token->quote == SINGLE_Q)
			token->content = ft_substr(token->content, 1, ft_strlen(token->content) - 2);
		else if (token->quote == DOUBLE_Q)
			token = quote_expend(token->content + 1, token->next, token->token_type);
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
	int			i;

	if (token->token_type == TK_REDIR_FILE
		&& *prev && (*prev)->token_type != TK_REDIR_FILE)
	{
		free(old_content);
		old_content = old_str(token);
	}
	token = cmd_handlers(token, prev);
	if (token && token->token_type == TK_REDIR_FILE && amb_error(prev, token, old_content))
		return (NULL);
	i = -1;
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
		&& token->next->content && token->next->content[0] != '\"')
			token = token ->next;
		if (token)
			token = token->next;
	}
	return (start);
}
