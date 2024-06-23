/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:20:57 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/23 17:13:12 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*here_doc_expand(char *text)
{
	int		i;
	int		j;
	int		size;
	char	*new;

	i = -1;
	size = 0;
	while (text[++i])
		if (text[i] == '$')
			value_fetcher(&text[i + 1], &size);
	new = (char *)malloc(sizeof(char) * (i + size + 1));
	if (!new)
		return (free(text), perror("malloc"), NULL);
	i = -1;
	j = -1;
	while (text[++j])
	{
		new[++i] = text[j];
		if (text[j] == '$')
			j += get_next_expand(&text[j + 1], new, &i);
		if (text[j + 1])
			break;
	}
	new[i] = '\0';
	return (free(text), new);
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
