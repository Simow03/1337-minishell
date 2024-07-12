/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:21:51 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/12 11:06:18 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_env(t_env **myenv)
{
	t_env	*tmp;

	(*myenv) = ft_env_lstnew("OLDPWD", NULL);
	tmp = ft_env_lstnew("PWD", getcwd(NULL, 0));
	ft_envadd_back(myenv, tmp);
	tmp = NULL;
	tmp = ft_env_lstnew("SHLVL", "1");
	ft_envadd_back(myenv, tmp);
	tmp = NULL;
	tmp = ft_env_lstnew("_", "/usr/bin/env");
	ft_envadd_back(myenv, tmp);
	tmp = NULL;
	ft_envadd_back(myenv, tmp);
	tmp = NULL;
	free(tmp);
}

void	fill_env(char **env, t_env **myenv, char **path, int i)
{
	char	*shlvl_val;
	t_env	*tmp;

	while (env[++i])
	{
		path = ft_envsplit(env[i]);
		if (!ft_strcmp(path[0], "SHLVL"))
		{
			shlvl_val = ft_itoa(ft_atoi(path[1]) + 1);
			free(path[1]);
			path[1] = shlvl_val;
		}
		if (!(*myenv))
			(*myenv) = ft_env_lstnew(path[0], path[1]);
		else
		{
			tmp = ft_env_lstnew(path[0], path[1]);
			ft_envadd_back(myenv, tmp);
		}
		free_dbl_str(path);
	}
}

void	add_var(char **env, t_env **myenv)
{
	char	**path;
	int		i;

	path = NULL;
	i = 0;
	while (env[i])
		i++;
	if (i == 0)
		return (init_env(myenv));
	i = -1;
	fill_env(env, myenv, path, i);
}

void	env(t_env	*var)
{
	while (var)
	{
		if (var->value)
			printf("%s=%s\n", var->name, var->value);
		var = var->next;
	}
	global_return_int(1, 0);
}
