/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 22:08:32 by mstaali           #+#    #+#             */
/*   Updated: 2024/06/06 16:41:02 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strdup(const char *str)
{
	char	*buffer;
	char	*b;

	if (!str)
		return (NULL);
	buffer = (char *)malloc(ft_strlen(str) + 1);
	if (!buffer)
		return (NULL);
	b = buffer;
	while (*str)
		*b++ = *str++;
	*b = '\0';
	return (buffer);
}
