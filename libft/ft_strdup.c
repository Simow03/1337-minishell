/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 22:08:32 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/02 20:25:47 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strdup(const char *str)
{
	char	*buffer;
	char	*b;

	buffer = (char *)malloc(ft_strlen(str) + 1);
	if (!buffer)
		return (NULL);
	b = buffer;
	while (*str)
		*b++ = *str++;
	*b = '\0';
	return (buffer);
}
