/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:03:51 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/09 18:50:17 by mstaali          ###   ########.fr       */
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

char	*get_old_path(t_env *iter)
{
	while (iter)
	{
		if (ft_strcmp(iter->name, "OLDPWD") == 0)
			return (ft_strdup(iter->value));
		iter = iter->next;
	}
	return (NULL);
}

char	*get_new_path(t_env *iter)
{
	while (iter)
	{
		if (ft_strcmp(iter->name, "PWD") == 0)
			return (ft_strdup(iter->value));
		iter = iter->next;
	}
	return (NULL);
}

int	cd_dash_option(char **cmd, t_env **myenv, char *old_pwd)
{
	char	*old_path;
	char	*curr_path;
	t_env	*iter;

	iter = *myenv;
	if (ft_strlen(cmd[1]) == 1)
	{
		old_path = get_old_path(iter);
		curr_path = get_new_path(iter);
		if (!old_path)
		{
			free(old_pwd);
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			return (global_return_int(1, 1));
		}
		if (chdir(old_path) == -1)
		{
			cd_error(old_path);
			return(free(old_pwd), global_return_int(1, 1));
		}
		printf("%s\n", old_path);
		while (iter)
		{
			if (ft_strcmp(iter->name, "PWD") == 0)
			{
				free(iter->value);
				iter->value = old_path;
				break;
			}
			iter = iter->next;
		}
		while (iter)
		{
			if (ft_strcmp(iter->name, "OLDPWD") == 0)
			{
				free(iter->value);
				iter->value = curr_path;
				break;
			}
			iter = iter->next;
		}
		free(old_pwd);
		return (global_return_int(1, 0));
	}
	else
		return (parse(cmd, myenv, old_pwd));
}
