/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 20:03:24 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/02 14:14:53 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_number(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i++;
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
	int	i;

	i = 0;
	while (cmd[i])
		i++;
	return (i);
}

void	decrement_shlvl(t_env **myenv)
{
	t_env	*tmp;
	int		shlvl_value;

	tmp = (*myenv);
	while (tmp)
	{
		if (ft_strcmp(tmp->name, "SHLVL") == 0)
		{
			shlvl_value = ft_atoi(tmp->value);
			if (shlvl_value <= 1)
				exit(0);
			tmp->value = ft_itoa(shlvl_value - 1);
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
			return (global_return_int(1, 1));
		}
		else
			exit((unsigned char)ft_exit_atoi(cmd[1]));
	}
	decrement_shlvl(myenv);
	return (global_return_int(1, 0));
}
