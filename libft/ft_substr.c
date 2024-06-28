/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 23:28:14 by mstaali           #+#    #+#             */
/*   Updated: 2024/06/28 09:55:30 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*str;
	size_t	size;

	if (!s)
		return (NULL);
	size = ft_strlen(s);
	if (size <= start)
		len = 0;
	else if (len > size - start)
		len = size - start;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	i = -1;
	while (++i < len)
	{
		str[i] = s[start + i];
	}
	str[i] = '\0';
	return (str);
}
