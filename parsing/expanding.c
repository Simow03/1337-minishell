/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:20:57 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/13 15:11:05 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*value_fetcher(char *text, t_env *env, int *size)
{
	size_t	i;

	i = 0;
	while (ft_isalpha(text[i]) || text[i] == '_')
		i++;
	if (!i && text[i] == '?')
		i++;
	if (!i)
		return (NULL);
	while (env)
	{
		if (ft_strlen(env->name) == i && !ft_strncmp(text, env->name, i))
		{
			if (size)
				*size += ft_strlen(env->value) - i - 1;
			return (env->value);
		}
		env = env->next;
	}
	if (size)
		*size -= i + 1;
	return (NULL);
}

int	sizeofexpndng(char *text, t_env *env)
{
	int		i;
	int		size;
	char	quote;

	i = -1;
	size = 0;
	quote = '\0';
	while (text[++i])
	{
		if (!quote && text[i] == '$'
			&& (text[i + 1] == '\"' || text[i + 1] == '\''))
			size--;
		else if (!quote && (text[i] == '\"' || text[i] == '\''))
		{
			size-=2;
			quote = text[i];
		}
		else if ((!quote || quote == '\"') && text[i] == '$')
			value_fetcher(&text[i + 1], env, &size);
		else if (quote == text[i])
			quote = '\0';
	}
	return (i + size);
}

int	get_next_expand(char *text, t_env *env, char *result, int *i)
{
	char	*value;
	int		check;
	int		j;

	check = 0;
	value = value_fetcher(text, env, &check);
	j = 0;
	while (ft_isalpha(text[j]) || text[j] == '_')
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

char	*expanding(char *text, t_env *env)
{
	char	*result;
	char	quote;
	int		i;
	
	result = (char *)malloc(sizeof(char) * (sizeofexpndng(text, env) + 1));
	if (!result)
		return (NULL);
	i = 0;
	quote = '\0';
	while (*text)
	{
		if (!quote && *text == '$'
			&& (*(text + 1) == '\"' || *(text + 1) == '\''))
			text++;
		if (!quote && (*text == '\"' || *text == '\''))
			quote = *text;
		else if ((!quote || quote == '\"') && *text == '$')
			text += get_next_expand(text + 1, env, result, &i);
		else if (quote == *text)
			quote = '\0';
		else
			result[i++] = *text;
		if (!(*(text++)))
			break;
	}
	result[i] = '\0';
	return (result);
}
