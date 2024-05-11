/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 20:03:24 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/05 23:20:53 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_number(char *str)
{
	int    i;

    i = 0;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

static int	count_args(char **cmd)
{
	int i;

    i = 0;
    while (cmd[i])
        i++;
    return (i);
}

int	builtin_exit(char **cmd, char *prompt)
{
	printf("exit\n");
	if (cmd[1])
	{
		if (!is_number(cmd[1]))
		{
			printf("%s: ", prompt);
			printf("exit: %s: numeric argument required", cmd[1]);
			exit(255);
		}
		if (count_args(cmd) > 2)
		{
			printf("%s: exit: too many arguments", prompt);
			return (1);
		}
		else
			exit((unsigned char)ft_atoi(cmd[1]));
	}
	return (0);
}
