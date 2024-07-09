/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 23:37:20 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/09 18:47:26 by mstaali          ###   ########.fr       */
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
			ft_putstr_fd("Not a directory\n", 2);
	}
	else
		ft_putstr_fd(": No such file or directory\n", 2);
	global_return_int(1, 1);
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
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (free(old_pwd), global_return_int(1, 1));
	}
	tmp = *myenv;
	while ((tmp))
	{
		if (ft_strcmp((tmp)->name, "OLDPWD") == 0)
		{
			free(tmp->value);
			(tmp)->value = old_pwd;
		}
		if (ft_strcmp((tmp)->name, "PWD") == 0)
		{
			free(tmp->value);
			(tmp)->value = ft_strdup(home);
		}
		(tmp) = (tmp)->next;
	}
	return (global_return_int(1, 0));
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
			return (cd_dash_option(cmd, myenv, old_pwd));
		else if (chdir(cmd[1]) == -1)
		{
			cd_error(cmd[1]);
			free(old_pwd);
			return (global_return_int(1, 1));
		}
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
				tmp->value = getcwd(NULL, 0);
			}
			tmp = tmp->next;
		}
		return (global_return_int(1, 0));
	}
	else
		return (get_home_dir(myenv, old_pwd));
}
