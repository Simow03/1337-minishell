/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 15:54:53 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/12 15:25:35 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*value_fetcher(char *text, int *size)
{
	size_t	i;
	t_env	*env;

	env = global_env(NULL, 0);
	i = 0;
	while (ft_isalpha(text[i]) || text[i] == '_')
		i++;
	if (size)
		*size = i;
	if (!i && text[i] == '?')
		return (global_return_str(0, 0));
	if (!i)
		return (NULL);
	while (env)
	{
		if (ft_strlen(env->name) == i && !ft_strncmp(text, env->name, i))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

t_token	*quote_expend(char *str, t_token *next, t_etoken token_type)
{
	t_token	*new;
	int		size;

	size = 0;
	if (str[0] == '\"')
		return (next);
	new = malloc(sizeof(t_token));
	new->token_type = token_type;
	new->quote = DOUBLE_Q;
	while (str[size] != '\"' && str[size] != '$')
		size++;
	if (*str == '$')
		new->content = value_fetcher(++str, &size);
	else
		new->content = ft_substr(str, 0, size);
	new->next = quote_expend(str + size, next, token_type);
	return (new);
}

t_token	*cmd_handlers(t_token *token, t_token **prev)
{
	char	*tmp;
	t_token	*t_tmp;

	if (token->token_type == TK_COMMAND || token->token_type == TK_REDIR_APND)
	{
		tmp = token->content;
		if (token->quote == NOT_Q && token->content && token->content[0] == '$')
			token->content = ft_strdup(value_fetcher(token->content + 1, NULL));
		else if (token->quote == SINGLE_Q)
			token->content = ft_substr(token->content, 1, ft_strlen(token->content) - 2);
		else if (token->quote == DOUBLE_Q)
		{
			t_tmp = token;
			token = quote_expend(token->content + 1, token->next, token->token_type);
			free(t_tmp);
			if (*prev)
				(*prev)->next = token;
		}
		else
			return (token);
		free(tmp);
	}
	return (token);
}

t_token	*cmd_join(t_token *token)
{
	t_token	*prev;
	t_token	*start;

	prev = NULL;
	while (token && token->token_type != TK_PIPE)
	{
		token = cmd_handlers(token, &prev);
		if (!prev)
			start = token;
		prev = token;
		while (token && token->quote == DOUBLE_Q && token->content
			&& token->content[0] != '\"')
			token = token ->next;
		if (token)
			token = token->next;
	}
	return (start);
}

static void token_printer(t_token *token)
{
	while (token)
	{
		printf("%d\t%s\n", token->token_type, token->content);
		token = token->next;
	}
}

int	cmd_size(t_token *token)
{
	int	size;
	int	is_full;

	size = 1;
	while (token && token->token_type != TK_PIPE)
	{
		if (token->token_type == TK_COMMAND)
		{
			is_full = 0;
			size++;
		}
		while (token && token->token_type == TK_COMMAND)
		{
			if (token->content)
				is_full = 1;
			token  = token->next;
		}
		if (is_full == 0)
			size--;
		if (token)
			token  = token->next;
	}
	return (size);
}

void	tree_branches(t_token *token, char **cmd)
{
	static int	i;
	char		tmp;

	tmp = NULL;
	if (token->token_type == TK_COMMAND)
	{
		while (token->token_type == TK_COMMAND)
			cmd[i] = ft_strjoin(tmp, token->content);
	}
}

t_tree	*tree_planting(t_token *token)
{
	t_tree	*tree;
	char	**cmd;

	tree = NULL;
	token = cmd_join(token);
	cmd = (char **)malloc(sizeof(char *) * (cmd_size(token) + 1));
	tree = (t_tree *)malloc(sizeof(t_tree));
	if (!cmd || !tree)
		return (perror("malloc"), NULL);
	tree_branches();
	token_printer(token);
	return (NULL);
}
