/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:21:51 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/16 13:56:02 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_var(char **env, t_env **myenv)
{
	char	**path;
	t_env    *tmp;
	int		i;
	int		j;

	if (!env)
		return ;
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
			tmp = NULL;
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
		if (var->value)
			printf("%s=%s\n", var->name, var->value);
		var = var->next;
	}
}
