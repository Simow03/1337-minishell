/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 00:47:34 by mstaali           #+#    #+#             */
/*   Updated: 2024/06/04 14:53:20 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_iswhitespace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' 
		|| c == '\v' || c == '\f');
}

static void	check_exit(long long result, char *str)
{
	if (result > (LLONG_MAX / 10) || (result < (LLONG_MIN / 10)))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
	}
}

int	ft_atoi(const char *str)
{
	int			i;
	int			sign;
	long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] && ft_iswhitespace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			sign *= (-1);
	}
	while (str[i])
		result = (result * 10) + (str[i++] - '0');
	return (result * sign);
}

int	ft_exit_atoi(char *str)
{
	int			sign;
	int			i;
	int			digit;
	long long	result;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] && ft_iswhitespace(str[i]))
		i++;
	if (str[i] == '+')
		i++;
	if (str[i] == '-' && i++)
		sign *= (-1);
	while (str[i])
	{
		digit = (str[i] - 48);
		check_exit(result, str);
		result = result * 10 + digit;
		i++;
	}
	return (result * sign);
}
