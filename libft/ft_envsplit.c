/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 20:39:43 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/02 21:53:42 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	**ft_envsplit(char const *s)
{
	char	**array;
	size_t	start;
	size_t	i;

	if (!s)
		return (NULL);
	array = (char **)malloc(3 * sizeof(char *));
	if (!array)
		return (NULL);
	i = 0;
	start = 0;
	while (s[i] && s[i] != '=')
		i++;
	array[0] = ft_substr(s, start, i - start);
    i++;
    array[1] = ft_substr(s, i, ft_strlen(s) - i);
	if (!array[0] || !array[1])
		return (ft_free_array(array));
	array[2] = NULL;
	return (array);
}
