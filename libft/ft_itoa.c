/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 20:46:16 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/13 16:38:09 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_count_digit(int n)
{
	int	count_digit;
	int	temp;

	temp = n;
	count_digit = 1;
	while ((temp / 10) != 0)
	{
		count_digit++;
		temp /= 10;
	}
	return (count_digit);
}

static int	ft_isnegative(int n)
{
	if (n < 0)
		return (1);
	return (0);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		sign;
	int		i;

	sign = 1;
	if (n < 0)
		sign = -1;
	result = (char *)malloc(ft_count_digit(n) + ft_isnegative(n) + 1);
	if (!result)
		return (NULL);
	if (sign == -1)
		result[0] = '-';
	i = ft_count_digit(n) + ft_isnegative(n);
	result[i] = '\0';
	if (n == 0)
		result[0] = '0';
	while (n != 0)
	{
		result[--i] = ((n % 10) * sign) + '0';
		n /= 10;
	}
	return (result);
}
