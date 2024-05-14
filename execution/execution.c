/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 21:03:00 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/14 01:32:45 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_builtin(char *cmd)
{
	if ((ft_strcmp(cmd, "echo") == 0 && ft_strlen(cmd) == 4) ||
	(ft_strcmp(cmd, "cd") == 0 && ft_strlen(cmd) == 2) ||
	(ft_strcmp(cmd, "pwd") == 0 && ft_strlen(cmd) == 3) ||
	(ft_strcmp(cmd, "export") == 0 && ft_strlen(cmd) == 6) ||
	(ft_strcmp(cmd, "unset") == 0 && ft_strlen(cmd) == 5) ||
	(ft_strcmp(cmd, "env") == 0 && ft_strlen(cmd) == 3) ||
	(ft_strcmp(cmd, "exit") == 0 && ft_strlen(cmd) == 4))
		return (1);
	return (0);
}

void	run_builtin(char **cmd, t_env **myenv)
{
	if (strcmp(cmd[0], "echo") == 0)
		echo(cmd);
	else if (ft_strcmp(cmd[0], "cd") == 0)
		cd(cmd, myenv);
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		pwd();
	else if (ft_strcmp(cmd[0], "export") == 0)
		export(myenv, cmd);
	//TODO: add unset function
	// else if (ft_strcmp(cmd[0], "unset") == 0)
	// 	unset(myenv, cmd);
	else if (ft_strcmp(cmd[0], "env") == 0)
		env(*myenv);
	else if (ft_strcmp(cmd[0], "exit") == 0)
		builtin_exit(cmd, myenv);
}

void	increment_shlvl(t_env **myenv)
{
	t_env    *tmp;

	tmp = (*myenv);
	while (tmp)
	{
		if (ft_strcmp(tmp->name, "SHLVL") == 0)
        {
            tmp->value = ft_itoa(ft_atoi(tmp->value) + 1);
            break ;
        }
        tmp = tmp->next;
	}
}

void	execute_cmd(char **cmd, t_env **myenv)
{
	int	pid;
	
	pid = fork();
	if (pid < 0)
		error_fork();
	if (pid == 0)
		check_args(cmd, myenv);
	//TODO: NULL needs to be replaced with &exit_status
	waitpid(pid, NULL, 0);
}

void	execution(t_tree *tree, t_env **myenv, char **env)
{
	char    **cmd;
	
	if (tree->node_type == 5)
		ft_pipe(tree, myenv, env);
	else if (tree->node_type == 1)
		left_redirect(tree, myenv, env);
	// else if (tree->node_type == 2)
	// 	left_double_redirect(tree, myenv, env);
	else if (tree->node_type == 3)
		right_redirect(tree, myenv, env);
	// else if (tree->node_type == 4)
	// 	right_double_redirect(tree, myenv, env);
	else if (tree->node_type == 0)
	{
		cmd = (char **)tree->content;
		if (is_builtin(cmd[0]))
			run_builtin(cmd, myenv);
		else if (ft_strcmp(cmd[0], "minishell") == 0 && !cmd[1])
			increment_shlvl(myenv);
		else
			execute_cmd(cmd, myenv);
	}
}
