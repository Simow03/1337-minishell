// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   tree.c                                             :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/05/06 14:33:10 by ayyassif          #+#    #+#             */
// /*   Updated: 2024/06/14 17:49:31 by ayyassif         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "../minishell.h"

t_token	*no_quote_expend(t_token *token, char *str, t_etoken token_type, t_token *next)
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
			new->next = no_quote_expend(token, str + size, token_type, next);
	}
	return (new);
}

t_token	*cmd_handlers(t_token *token, t_token **prev)
{
	t_token	*t_tmp;

	if (token->token_type == TK_COMMAND || token->token_type == TK_REDIR_FILE)
	{
		t_tmp = token;
		if (token->quote == NOT_Q && token->content && token->content[0] == '$')
		{
			token = no_quote_expend(token, value_fetcher(token->content + 1, NULL),
				token->token_type, token->next);
			free(t_tmp);
		}
		else if (token->quote == SINGLE_Q)
			token->content = ft_substr(token->content, 1, ft_strlen(token->content) - 2);
		else if (token->quote == DOUBLE_Q)
		{
			token = quote_expend(token->content + 1, token->next, token->token_type);
			free(t_tmp);
		}
		if (*prev)
			(*prev)->next = token;
	}
	return (token);
}

t_token	*amb_error(t_token **prev, t_token *token, char *old_content)
{
	char		*err_msg;
	static int	is_full;

	if (token)
	{
		if (*prev && (*prev)->token_type != TK_REDIR_FILE)
			is_full = 0;
		if (token->content)
			is_full = 1;
		if (((!token->next || token->next->token_type != TK_REDIR_FILE) && is_full) ||
			(token->next && token->next->token_type == TK_REDIR_FILE))
			return (token);
	}
	err_msg = error_printer(3, old_content);
	ft_putstr_fd(err_msg, STDERR_FILENO);
	free(err_msg);
	return (NULL);
}

int	cmd_size(t_token *token)
{
	int	size;
	int	is_full;

	size = 0;
	while (token && token->token_type != TK_PIPE)
	{
		is_full = 1;
		if (token->token_type == TK_COMMAND)
		{
			is_full = 0;
			size++;
		}
		while (token->token_type == TK_COMMAND
			&& token->next && token->next->token_type == TK_COMMAND)
		{
			if (token->content)
				is_full = 1;
			token  = token->next;
		}
		if (!token->content && is_full == 0)
			size--;
		if (token)
			token  = token->next;
	}
	return (size);
}

t_tree	*cmd_tree(char	**cmd)
{
	t_tree		*new;

	if (!cmd[0])
	{
		free(cmd);
		return (NULL);
	}
	new = (t_tree *)malloc(sizeof(t_tree));
	if (!new)
		return (perror("malloc"), NULL);
	new->left = NULL;
	new->right = NULL;
	new->node_type = TR_COMMAND;
	new->content = (void *)cmd;
	return (new);
}
