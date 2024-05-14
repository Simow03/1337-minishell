/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:20:57 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/11 16:01:19 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*value_fetcher(char *token, t_env *env, int *size)
{
	size_t	i;

	i = 0;
	while (ft_isalpha(token[i]) || token[i] == '_')
		i++;
	if (!i)
		return (NULL);
	while (env)
	{
		if (ft_strlen(env->name) == i && !ft_strncmp(token, env->name, i))
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

int	sizeofexpndng(char *token, t_env *env)
{
	int		i;
	int		size;
	char	quote;

	i = -1;
	size = 0;
	quote = '\0';
	while (token[++i])
	{
		if (!quote && (token[i] == '\"' || token[i] == '\''))
		{
			size-=2;
			quote = token[i];
		}
		else if ((!quote || quote == '\"') && token[i] == '$')
			value_fetcher(&token[i + 1], env, &size);
		else if (quote == token[i])
			quote = '\0';
	}
	return (i + size);
}

int	get_next_expand(char *token, t_env *env, char *result, int *i)
{
	char	*value;
	int		check;
	int		j;

	check = 0;
	value = value_fetcher(token, env, &check);
	j = 0;
	while (ft_isalpha(token[j]) || token[j] == '_')
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

char	*expanding(char *token, t_env *env)
{
	char	*result;
	char	quote;
	int		i;
	
	result = (char *)malloc(sizeof(char) * (sizeofexpndng(token, env) + 1));
	if (!result)
		return (NULL);
	i = 0;
	quote = '\0';
	while (*token)
	{
		if (!quote && (*token == '\"' || *token == '\''))
			quote = *token;
		else if ((!quote || quote == '\"') && *token == '$')
			token += get_next_expand(token + 1, env, result, &i);
		else if (quote == *token)
			quote = '\0';
		else
			result[i++] = *token;
		if (!(*(token++)))
			break;
	}
	result[i] = '\0';
	return (result);
}
