/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:22:28 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/01 22:59:37 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	echo(char **cmd)
{
	int	i;

	i = 1;
	if (ft_strncmp(cmd[1], "-n", 2) == 0 && ft_strlen(cmd[1]) == 2)
		i = 2;
    while (cmd[i])
    {
        ft_putstr_fd(cmd[i], 1);
        if (cmd[i + 1])
            ft_putstr_fd(" ", 1);
        i++;
    }
	if (ft_strncmp(cmd[1], "-n", 2) != 0 && ft_strlen(cmd[1]) == 2)
    	ft_putstr_fd("\n", 1);
}
