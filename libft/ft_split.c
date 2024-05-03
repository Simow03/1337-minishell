/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:32:20 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/02 17:32:35 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_countsubstr(const char *s, char c)
{
	int		found;
	int		count;

	found = 0;
	count = 0;
	while (*s)
	{
		if (*s != c)
		{
			if (!found)
			{
				count++;
				found = 1;
			}
		}
		else
			found = 0;
		s++;
	}
	return (count);
}

static char	**ft_free_array(char **array)
{
	size_t	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	size_t	arr_i;
	size_t	start;
	size_t	i;

	if (!s)
		return (NULL);
	array = (char **)malloc((ft_countsubstr(s, c) + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	i = 0;
	arr_i = 0;
	while (s[i])
	{
		if (s[i] && s[i] == c && ++i)
			continue ;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		array[arr_i++] = ft_substr(s, start, i - start);
		if (!array[arr_i - 1])
			return (ft_free_array(array));
	}
	array[arr_i] = NULL;
	return (array);
}
