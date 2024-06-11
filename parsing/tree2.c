/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 15:54:53 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/11 10:32:18 by ayyassif         ###   ########.fr       */
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

t_token	*quote_expend(char *str, t_token *next)
{
	t_token	*new;
	int		size;

	size = 0;
	if (str[0] == '\"')
		return (next);
	new = malloc(sizeof(t_token));
	new->token_type = TK_COMMAND;
	new->quote = NOT_Q;
	while (str[size] != '\"' && str[size] != '$')
		size++;
	if (*str == '$')
		new->content = value_fetcher(++str, &size);
	else
		new->content = ft_substr(str, 0, size);
	new->next = quote_expend(str + size, next);
	return (new);
}

t_token	*cmd_handlers(t_token *token, t_token **prev)
{
	char	*tmp;
	t_token	*t_tmp;

	tmp = token->content;
	if (token->token_type == TK_COMMAND && token->quote == NOT_Q
		&& token->content && token->content[0] == '$')
		token->content = ft_strdup(value_fetcher(token->content + 1, NULL));
	else if (token->token_type == TK_COMMAND && token->quote == DOUBLE_Q)
	{
		t_tmp = token;
		token = quote_expend(token->content + 1, token->next);
		free(t_tmp);
		if (*prev)
			(*prev)->next = token;
	}
	else if (token->token_type == TK_COMMAND && token->quote == SINGLE_Q)
		token->content = ft_substr(token->content, 1, ft_strlen(token->content) - 2);
	free(tmp);
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

t_tree	*tree_planting(t_token *token)
{
	// t_tree	*tree;
	// //char	**cmd;
	// int		i;

	// tree = NULL;
	// //cmd = malloc(sizeof(char) * (cmd_size(token) + 1));
	// if (!cmd)
	// 	return (perror("malloc"), NULL);
	// i = 0;
	token = cmd_join(token);
	token_printer(token);
	return (NULL);
}