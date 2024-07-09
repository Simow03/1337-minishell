/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:22:28 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/09 18:53:10 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_flag(char *cmd)
{
	int	i;

	if (cmd[0] != '-')
		return (0);
	i = 1;
	while (cmd[i] && cmd[i] == 'n')
		i++;
	if (!cmd[i])
		return (1);
	return (0);
}

void	echo(char **cmd)
{
	int	i;

	i = 1;
	if (cmd[1])
	{
		while (is_flag(cmd[i]))
			i++;
		while (cmd[i])
		{
			ft_putstr_fd(cmd[i], 1);
			if (cmd[i + 1])
				ft_putstr_fd(" ", 1);
			i++;
		}
		if (!is_flag(cmd[1]))
			ft_putstr_fd("\n", 1);
	}
	else
		ft_putstr_fd("\n", 1);
	global_return_int(1, 0);
}
