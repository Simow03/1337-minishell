/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:20:57 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/30 12:36:26 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*here_doc_expand(char *str)
{
	t_token	*new;
	int		size;
	char	*tmp;

	tmp = str;
	size = 0;
	new = (t_token *)malloc(sizeof(t_token));
	new->token_type = TK_HRDC_CONTENT;
	new->quote = NOT_Q;
	if (!str || !str[0])
	{
		new->content = ft_strdup("");
		new->next = NULL;
		return (new);
	}
	while (str[size] && str[size] != '$')
		size++;
	if (str[0] == '$')
		new->content = ft_strdup(value_fetcher(++str, &size));
	else
		new->content = ft_substr(str, 0, size);
	new->next = here_doc_expand(str + size);
	free(tmp);
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
