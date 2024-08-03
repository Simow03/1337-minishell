/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 23:37:20 by mstaali           #+#    #+#             */
/*   Updated: 2024/08/03 16:39:15 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	change_pwd(t_env *tmp, char *old_pwd, char *home, int flag)
{
	while (tmp)
	{
		if (ft_strcmp(tmp->name, "OLDPWD") == 0)
		{
			free(tmp->value);
			tmp->value = old_pwd;
		}
		if (ft_strcmp(tmp->name, "PWD") == 0)
		{
			free(tmp->value);
			if (flag == 0)
				tmp->value = getcwd(NULL, 0);
			else if (flag == 1)
				tmp->value = ft_strdup(home);
		}
		tmp = tmp->next;
	}
}

int	get_home_dir(t_env **myenv, char *old_pwd)
{
	char	*home;
	t_env	*tmp;

	tmp = *myenv;
	while (tmp)
	{
		if (ft_strcmp((tmp)->name, "HOME") == 0)
		{
			home = (tmp)->value;
			break ;
		}
		(tmp) = (tmp)->next;
	}
	if (chdir(home) == -1)
	{
		ft_putstr_fd("Minishell: cd: HOME not set\n", 2);
		return (free(old_pwd), global_return_int(1, 1));
	}
	tmp = *myenv;
	change_pwd(tmp, old_pwd, home, 1);
	return (global_return_int(1, 0));
}

int	cd(char **cmd, t_env **myenv)
{
	char	*old_pwd;
	t_env	*tmp;

	tmp = *myenv;
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		cwd_error();
	if (cmd[1])
	{
		if (cmd[1][0] == '-')
			return (cd_dash_option(cmd, myenv, old_pwd));
		else if (chdir(cmd[1]) == -1)
		{
			cd_error(cmd[1]);
			free(old_pwd);
			return (global_return_int(1, 1));
		}
		change_pwd(tmp, old_pwd, NULL, 0);
		return (global_return_int(1, 0));
	}
	else
		return (get_home_dir(myenv, old_pwd));
}
