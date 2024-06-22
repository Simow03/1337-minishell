/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:20:57 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/22 15:56:20 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*cmd_join(t_token *token)
{
	t_token	*prev;
	t_token	*start;

	prev = NULL;
	start = token;
	while (token && token->token_type != TK_PIPE)
	{
		token = cmd_join_util(&prev, token);
		if (!token)
		{
			free(start);
			return (NULL);
		}
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

char	*here_doc_expand(char *text)
{
	int		i;
	int		size;
	char	*new;

	i = -1;
	size = 0;
	while (text[++i])
		if (text[i] == '$')
			value_fetcher(&text[i + 1], &size);
	new = (char *)malloc(sizeof(char) * (i + size + 1));
	if (!new)
		return (perror("malloc"), NULL);
	i = 0;
	while (*text)
	{
		if (*text == '$')
			text += get_next_expand(text + 1, new, &i);
		else
			new[i++] = *text;
		if (!(*(text++)))
			break;
	}
	new[i] = '\0';
	return (new);
}

char	*value_fetcher(char *text, int *size)
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
	new = (t_token *)malloc(sizeof(t_token));
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

int	get_next_expand(char *text, char *result, int *i)
{
	char	*value;
	int		check;
	int		j;

	check = 0;
	value = value_fetcher(text, &check);
	j = 0;
	while (ft_isalpha(text[j]) || text[j] == '_')
		j++;
	if (!j && text[j] == '?')
		j++;
	if (!value)
	{
		if (!check)
			result[(*i)++] = '$';
		return (j);
	}
	while (*value)
		result[(*i)++] = *(value++);
	return (j);
}
