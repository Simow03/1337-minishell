/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 01:13:49 by mstaali           #+#    #+#             */
/*   Updated: 2024/08/03 19:12:45 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*check_env(t_env *myenv)
{
	while (myenv)
	{
		if (ft_strcmp(myenv->name, "PATH") == 0)
			return (myenv->value);
		myenv = myenv->next;
	}
	return (NULL);
}

char	*find_path(char *cmd, t_env *myenv)
{
	char	**paths;
	char	*path;
	char	*b4_cmd;
	int		i;

	while (ft_strcmp(myenv->name, "PATH") != 0)
		myenv = myenv->next;
	paths = ft_split(myenv->value, ':');
	i = 0;
	while (paths[i])
	{
		b4_cmd = ft_strjoin(paths[i], "/");
		path = ft_strjoin(b4_cmd, cmd);
		free(b4_cmd);
		if (access(path, F_OK & X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (NULL);
}

void	check_args(char **cmd, t_env **myenv)
{
	char		*path;
	struct stat	buff;

	stat(cmd[0], &buff);
	if (S_ISDIR(buff.st_mode))
	{
		error_folder(cmd[0]);
		return ;
	}
	create_str_env(*myenv);
	if ((ft_strncmp(cmd[0], "./", 2) == 0 || cmd[0][0] == '/')
		&& access(cmd[0], F_OK & X_OK) == 0
		&& execve(cmd[0], cmd, (*myenv)->str_env) == -1)
		error_permission(cmd[0]);
	if (!check_env(*myenv))
		error_path(cmd[0]);
	path = find_path(cmd[0], *myenv);
	if (!path)
		error_cmd(cmd[0]);
	if (execve(path, cmd, (*myenv)->str_env) == -1)
		error_cmd(cmd[0]);
}

void	replace_last_cmd(char **cmd, t_env **myenv, char *flag)
{
	t_env	*tmp;
	char	*new_value;

	tmp = *myenv;
	new_value = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, "_") == 0)
		{
			if (ft_strcmp(flag, "builtin") == 0)
				new_value = ft_strdup(cmd[0]);
			else if (ft_strcmp(flag, "cmd") == 0)
				new_value = ft_strdup(cmd[ft_dbl_strlen(cmd) - 1]);
			if (new_value)
			{
				free(tmp->value);
				tmp->value = new_value;
			}
			return ;
		}
		tmp = tmp->next;
	}
}
