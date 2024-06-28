/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_options.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:03:51 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/16 15:25:30 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parse(char **cmd, t_env **myenv, char *old_pwd)
{
	if (ft_strcmp(cmd[1], "--") == 0)
		get_home_dir(myenv, old_pwd);
	else if (cmd[1][1] != '-' || ft_strlen(cmd[1]) > 2)
	{
		ft_putstr_fd("minishel: cd: -", 2);
		ft_putchar_fd(cmd[1][1], 2);
		ft_putstr_fd(": invalid option\n", 2);
		ft_putstr_fd("cd: usage: cd [-L|-P] [dir]\n", 2);
	}
}

char	*get_old_path(t_env *iter)
{
	while (iter)
	{
		if (ft_strcmp(iter->name, "OLDPWD") == 0)
			return (iter->value);
		iter = iter->next;
	}
	return (NULL);
}

void	cd_dash_option(char **cmd, t_env **myenv, char *old_pwd)
{
	char	*tmp_path;
	t_env	*iter;

	iter = *myenv;
	if (ft_strlen(cmd[1]) == 1)
	{
		tmp_path = get_old_path(iter);
		if (chdir(tmp_path) == -1)
			cd_error(tmp_path);
		printf("%s\n", getcwd(NULL, 0));
		while (iter)
		{
			if (ft_strcmp(iter->name, "OLDPWD") == 0)
				iter->value = getcwd(NULL, 0);
			if (ft_strcmp(iter->name, "PWD") == 0)
				iter->value = tmp_path;
			iter = iter->next;
		}
	}
	else
		parse(cmd, myenv, old_pwd);
}
