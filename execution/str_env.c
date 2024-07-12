/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 09:21:42 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/12 09:24:33 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_envjoin(const char *s1, const char *s2)
{
	char	*buffer;
	char	*b;
	size_t	total_len;

	if (!s1 || !s2)
		return (NULL);
	total_len = ft_strlen(s1) + ft_strlen(s2) + 2;
	buffer = (char *)malloc(total_len);
	if (!buffer)
		return (NULL);
	b = buffer;
	while (*s1)
		*b++ = *s1++;
	*b++ = '=';
	while (*s2)
		*b++ = *s2++;
	*b = '\0';
	return (buffer);
}

void	fill_str_env(t_env *myenv, t_env *tmp)
{
	int	i;

	i = 0;
	while (tmp)
	{
		if (tmp->value)
		{
			if (!tmp->name || !tmp->value)
				return ;
			myenv->str_env[i] = ft_envjoin(tmp->name, tmp->value);
			if (!myenv->str_env[i])
				return ;
			i++;
		}
		tmp = tmp->next;
	}
	myenv->str_env[i] = NULL;
}

void	create_str_env(t_env *myenv)
{
	t_env	*tmp;
	int		count;

	if (!myenv)
		return ;
	tmp = myenv;
	count = 0;
	while (tmp)
	{
		if (tmp->value)
			count++;
		tmp = tmp->next;
	}
	myenv->str_env = (char **)malloc((count + 1) * sizeof(char *));
	if (!myenv->str_env)
		return ;
	tmp = myenv;
	fill_str_env(myenv, tmp);
}
