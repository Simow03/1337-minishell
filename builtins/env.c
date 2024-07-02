/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:21:51 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/02 14:56:12 by ayyassif         ###   ########.fr       */
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
	tmp = ft_env_lstnew("PATH", "/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.");
	tmp->is_hidden = 1;
	ft_envadd_back(myenv, tmp);
	tmp = NULL;
	free(tmp);
}

void	free_tenv(t_env *var)
{
	if (var != NULL)
	{
		if (var->name)
			free(var->name);
		if (var->value)
			free(var->value);
		free(var);
	}
}

void	add_var(char **env, t_env **myenv)
{
	char	**path;
	t_env	*tmp;
	int		i;
	int		j;

	i = 0;
	while (env[i])
		i++;
	if (i == 0)
		return(init_env(myenv));
	i = -1;
	while (env[++i])
	{
		path = ft_envsplit(env[i]);
		if (!(*myenv))
			(*myenv) = ft_env_lstnew(path[0], path[1]);
		else
		{
			tmp = ft_env_lstnew(path[0], path[1]);
			ft_envadd_back(myenv, tmp);
		}
		j = 0;
		while (path[j])
			free(path[j++]);
		free(path);
	}
}

void	env(t_env	*var)
{
	while (var)
	{
		if (var->value && var->is_hidden != 1)
			printf("%s=%s\n", var->name, var->value);
		var = var->next;
	}
	global_return_int(1, 0);
}
