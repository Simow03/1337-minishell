/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_errors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 09:41:07 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/15 16:33:19 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parse(char **cmd, t_env **myenv, char *old_pwd)
{
	if (ft_strcmp(cmd[1], "--") == 0)
		return (get_home_dir(myenv, old_pwd));
	else if (cmd[1][1] != '-' || ft_strlen(cmd[1]) > 2)
	{
		ft_putstr_fd("minishel: cd: -", 2);
		ft_putchar_fd(cmd[1][1], 2);
		ft_putstr_fd(": invalid option\n", 2);
		ft_putstr_fd("cd: usage: cd [-L|-P] [dir]\n", 2);
		free(old_pwd);
		return (global_return_int(1, 1));
	}
	return (0);
}

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

int	check_old_path(char *old_path, char *old_pwd)
{
	if (!old_path)
	{
		free(old_pwd);
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		global_return_int(1, 1);
		return (0);
	}
	if (chdir(old_path) == -1)
	{
		cd_error(old_path);
		free(old_pwd);
		global_return_int(1, 1);
		return (0);
	}
	return (1);
}
