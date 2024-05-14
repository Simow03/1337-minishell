/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 20:03:24 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/13 21:55:58 by mstaali          ###   ########.fr       */
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

void	decrement_shlvl(t_env **myenv)
{
	t_env    *tmp;

	tmp = (*myenv);
	while (tmp)
	{
		if (ft_strcmp(tmp->name, "SHLVL") == 0)
        {
			if (ft_strcmp(tmp->value, "1") == 0)
				exit(0);
            tmp->value = ft_itoa(ft_atoi(tmp->value) - 1);
            break ;
        }
        tmp = tmp->next;
	}
}

int	builtin_exit(char **cmd, t_env **myenv)
{
	printf("exit\n");
	if (cmd[1])
	{
		if (!is_number(cmd[1]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(cmd[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			decrement_shlvl(myenv);
			exit(255);
		}
		if (count_args(cmd) > 2)
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return (1);
		}
		else
		{
			decrement_shlvl(myenv);
			exit((unsigned char)ft_atoi(cmd[1]));
		}
	}
	decrement_shlvl(myenv);
	return (0);
}
