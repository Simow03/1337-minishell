/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 15:21:21 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/08 18:18:29 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env(t_env	*env)
{
	free(env->name);
	free(env->value);
	if (env->next)
		free_env(env->next);
	free(env);
}

t_env	*new_env(char *line)
{
	t_env	*new;
	int		i;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (perror("malloc"), NULL);
	i = 0;
	while (line[i] && line[i] != '=')
		i++;
	new->name = ft_substr(line, 0, i);
    i++;
    new->value = ft_substr(line, i, ft_strlen(line) - i);
	if (!new->name | !new->value)
	{
		free(new->name);
		free(new->value);
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}

int	add_env_back(t_env	**env, t_env *new)
{
	t_env	*tmp;

	if (!new)
		return (1);
	if (!ft_strncmp(new->name, "OLDPWD", 6))
	{
		free(new->name);
		free(new->value);
		free(new);
		return (0);
	}
	if (!(*env))
	{
		*env = new;
		return (0);
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (0);
}

t_env	*create_env(char **env)
{
	t_env	*myenv;
	int		i;

	myenv = NULL;
	i = -1;
	while (env[++i])
	{
		if (add_env_back(&myenv, new_env(env[i])))
			return (NULL);
	}
	return (myenv);
}
