/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 23:37:20 by mstaali           #+#    #+#             */
/*   Updated: 2024/06/07 16:42:08 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cd_error(char *path)
{
	struct stat	path_stat;

	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	if (stat(path, &path_stat) == 0)
	{
		if (S_ISREG(path_stat.st_mode))
			ft_putstr_fd(": Not a directory\n", 2);
	}
	else
		ft_putstr_fd(": No such file or directory\n", 2);
	global_return_int(1, 1);
}

void	get_home_dir(t_env **myenv, char *old_pwd)
{
	char	*home;
	t_env	*tmp;

	tmp = *myenv;
	while (tmp)
	{
		if (ft_strcmp((tmp)->name, "HOME") == 0)
			home = (tmp)->value;
		(tmp) = (tmp)->next;
	}
	if (chdir(home) == -1)
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
	tmp = *myenv;
	while ((tmp))
	{
		if (ft_strcmp((tmp)->name, "OLDPWD") == 0)
			(tmp)->value = old_pwd;
		if (ft_strcmp((tmp)->name, "PWD") == 0)
			(tmp)->value = home;
		(tmp) = (tmp)->next;
	}
	global_return_int(1, 0);
}

int	cd(char **cmd, t_env **myenv)
{
	char	*old_pwd;
	t_env	*tmp;

	tmp = *myenv;
	old_pwd = getcwd(NULL, 0);
	if (cmd[1])
	{
		if (cmd[1][0] == '-')
			cd_dash_option(cmd, myenv, old_pwd);
		else if (chdir(cmd[1]) == -1)
			cd_error(cmd[1]);
		while (tmp)
		{
			if (ft_strcmp(tmp->name, "OLDPWD") == 0)
				tmp->value = old_pwd;
			if (ft_strcmp(tmp->name, "PWD") == 0)
				tmp->value = getcwd(NULL, 0);
			tmp = tmp->next;
		}
	}
	else
		get_home_dir(myenv, old_pwd);
	return (global_return_int(1, 0));
}
