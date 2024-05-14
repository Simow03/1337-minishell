/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 01:13:49 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/14 01:31:27 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*check_env(t_env *myenv)
{
	while (myenv)
	{
		if (strcmp(myenv->name, "PATH") == 0)
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
	char	*path;

	if (access(cmd[0], F_OK & X_OK) == 0)
			if (execve(cmd[0], cmd, NULL) == -1)
				error_cmd(cmd[0]);
	if (!check_env(*myenv))
		error_cmd(cmd[0]);
	path = find_path(cmd[0], *myenv);
	if (!path)
		error_cmd(cmd[0]);
	if (execve(path, cmd, NULL) == -1)
		error_cmd(cmd[0]);
}
