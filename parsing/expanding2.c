/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:47:11 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/22 16:49:02 by ayyassif         ###   ########.fr       */
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

int	sizeofexpndng(char *text)
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
			value_fetcher(&text[i + 1], global_env(NULL, 0), &size);
		else if (quote == text[i])
			quote = '\0';
	}
	return (i + size);
}

int	get_next_expand(char *text, char *result, int *i)
{
	char	*value;
	int		check;
	int		j;

	check = 0;
	value = value_fetcher(text, global_env(NULL, 0), &check);
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

int	ambiguous(char *text, char quote, int is_ambiguous)
{
	int		size;
	char	*str;
	
	if (!text)
	{
		if (is_ambiguous == 1)
			return (0);
		return (is_ambiguous);
	}
	size = 0;
	str = value_fetcher(text, global_env(NULL, 0), &size);
	if (!size && !str)
		is_ambiguous = 0;
	if (str)
	{
		is_ambiguous = 0;
		while (!quote && *str)
			if (*(str++) == ' ')
				is_ambiguous = 2;
	}
	return (is_ambiguous);
}
