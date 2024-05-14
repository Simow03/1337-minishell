/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 23:37:20 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/13 01:23:34 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	get_home_dir(t_env **env, char *old_pwd)
{
	char	*home;

	while ((*env))
	{
		if (ft_strcmp((*env)->name, "HOME") == 0)
            home = (*env)->value;
        (*env) = (*env)->next;
	}
	while ((*env))
	{
		if (ft_strcmp((*env)->name, "OLDPWD") == 0)
            (*env)->value = old_pwd;
        if (ft_strcmp((*env)->name, "PWD") == 0)
            (*env)->value = home;
        (*env) = (*env)->next;
	}
}

int	cd(char **cmd, t_env **env)
{
	char	*curr_pwd;
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (cmd[1])
	{
        if (chdir(cmd[1]) == -1)
		{
            ft_putstr_fd("minishell: cd: ", 2);
            ft_putstr_fd(cmd[1], 2);
            ft_putstr_fd(": No such file or directory\n", 2);
			return (1);
		}
		curr_pwd = cmd[1];
		while ((*env))
		{
			if (ft_strcmp((*env)->name, "OLDPWD") == 0)
                (*env)->value = old_pwd;
            if (ft_strcmp((*env)->name, "PWD") == 0)
                (*env)->value = curr_pwd;
            (*env) = (*env)->next;
		}
    }
	else
		get_home_dir(env, old_pwd);
	return (0);
}
