/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 23:37:20 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/15 19:31:03 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cd_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", 2);
    ft_putstr_fd(path, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	return (1);
}

int	get_home_dir(t_env **env, char *old_pwd)
{
	char	*home;
	t_env	*tmp;
	
	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp((tmp)->name, "HOME") == 0)
            home = (tmp)->value;
        (tmp) = (tmp)->next;
	}
	if (chdir(home) == -1)
		cd_error(home);
	tmp = *env;
	while ((tmp))
	{
		if (ft_strcmp((tmp)->name, "OLDPWD") == 0)
            (tmp)->value = old_pwd;
        if (ft_strcmp((tmp)->name, "PWD") == 0)
            (tmp)->value = home;
        (tmp) = (tmp)->next;
	}
	return (0);
}

// cd_options()
// {
	
// }

int	cd(char **cmd, t_env **env)
{
	char	*curr_pwd;
	char	*old_pwd;
	t_env	*tmp;

	tmp  = *env;
	old_pwd = getcwd(NULL, 0);
	if (cmd[1])
	{
		// if (ft_strcmp(cmd[1],"-") || ft_strcmp(cmd[1],"\\"))
		// 	cd_options();
        if (chdir(cmd[1]) == -1)
			cd_error(cmd[1]);
		curr_pwd = cmd[1];
		while (tmp)
		{
			if (ft_strcmp(tmp->name, "OLDPWD") == 0)
                tmp->value = old_pwd;
            if (ft_strcmp(tmp->name, "PWD") == 0)
                (*env)->value = curr_pwd;
            tmp = tmp->next;
		}
    }
	else
		get_home_dir(env, old_pwd);
	return (0);
}
