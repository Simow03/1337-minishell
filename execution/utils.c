/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 01:13:49 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/11 15:56:38 by mstaali          ###   ########.fr       */
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

char	*ft_envjoin(const char *s1, const char *s2)
{
	char	*buffer;
	char	*b;
	size_t	total_len;

	if (!s1 || !s2)
		return (NULL);
	total_len = ft_strlen(s1) + ft_strlen(s2) + 2;
	buffer = (char *)malloc(total_len);
	if (!buffer)
		return (NULL);
	b = buffer;
	while (*s1)
		*b++ = *s1++;
	*b++ = '='; 
	while (*s2)
		*b++ = *s2++;
	*b = '\0';
	return (buffer);
}

void create_str_env(t_env *myenv)
{
    t_env	*tmp;
    int		count;
	int		i;

	if (!myenv)
		return ;
	tmp = myenv;
	count = 0;
	while (tmp)
	{
		if (tmp->value)
			count++;
		tmp = tmp->next;
	}
	myenv->str_env = (char **)malloc((count + 1) * sizeof(char *));
	if (!myenv->str_env)
		return ;
	tmp = myenv;
	i = 0;
	while (tmp)
	{
		if (tmp->value)
		{
			if (!tmp->name || !tmp->value)
                return;
			myenv->str_env[i] = ft_envjoin(tmp->name, tmp->value);
			if (!myenv->str_env[i])
				return;
			i++;
		}
		tmp = tmp->next;
	}
	myenv->str_env[i] = NULL;
}

void	check_args(char **cmd, t_env **myenv)
{
	char	*path;

	create_str_env(*myenv);
	if (access(cmd[0], F_OK & X_OK) == 0)
		if (execve(cmd[0], cmd, (*myenv)->str_env) == -1)
			error_cmd(cmd[0]);
	if (ft_strchr(cmd[0], '/') == 0)
		error_path(cmd[0]);
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
