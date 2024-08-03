/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:03:51 by mstaali           #+#    #+#             */
/*   Updated: 2024/08/03 12:08:57 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	swap_path(t_env *myenv, t_env *iter, char *old_path, char *curr_path)
{
	while (iter)
	{
		if (ft_strcmp(iter->name, "PWD") == 0)
		{
			free(iter->value);
			iter->value = old_path;
			break ;
		}
		iter = iter->next;
	}
	iter = myenv;
	while (iter)
	{
		if (ft_strcmp(iter->name, "OLDPWD") == 0)
		{
			free(iter->value);
			iter->value = curr_path;
			break ;
		}
		iter = iter->next;
	}
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
		if (!check_old_path(old_path, old_pwd))
		{
			free(curr_path);
			return (1);
		}
		printf("%s\n", old_path);
		swap_path(*myenv, iter, old_path, curr_path);
		free(old_pwd);
		return (global_return_int(1, 0));
	}
	else
		return (parse(cmd, myenv, old_pwd));
}
