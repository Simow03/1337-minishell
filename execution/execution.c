/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 21:03:00 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/16 20:54:39 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_builtin(char *cmd)
{
	if ((ft_strcmp(cmd, "echo") == 0 && ft_strlen(cmd) == 4)
		|| (ft_strcmp(cmd, "cd") == 0 && ft_strlen(cmd) == 2)
		|| (ft_strcmp(cmd, "pwd") == 0 && ft_strlen(cmd) == 3)
		|| (ft_strcmp(cmd, "export") == 0 && ft_strlen(cmd) == 6)
		|| (ft_strcmp(cmd, "unset") == 0 && ft_strlen(cmd) == 5)
		|| (ft_strcmp(cmd, "env") == 0 && ft_strlen(cmd) == 3)
		|| (ft_strcmp(cmd, "exit") == 0 && ft_strlen(cmd) == 4))
		return (1);
	return (0);
}

void	run_builtin(char **cmd, t_env **myenv)
{
	replace_last_cmd(cmd, myenv, "builtin");
	if (strcmp(cmd[0], "echo") == 0)
		echo(cmd);
	else if (ft_strcmp(cmd[0], "cd") == 0)
		cd(cmd, myenv);
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		pwd();
	else if (ft_strcmp(cmd[0], "export") == 0)
		export(myenv, cmd);
	else if (ft_strcmp(cmd[0], "unset") == 0)
		unset(myenv, cmd);
	else if (ft_strcmp(cmd[0], "env") == 0)
		env(*myenv);
	else if (ft_strcmp(cmd[0], "exit") == 0)
		builtin_exit(cmd);
}

void	execute_cmd(char **cmd, t_env **myenv)
{
	int	pid;
	int	status;

	signal(SIGINT, exec_signo);
	signal(SIGQUIT, exec_signo);
	replace_last_cmd(cmd, myenv, "cmd");
	pid = fork();
	if (pid < 0)
		error_fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		check_args(cmd, myenv);
	}
	waitpid(pid, &status, 0);
	get_status(status);
}

int	check_and_operator(char **cmd)
{
	int	i;

	i = -1;
	while (cmd[++i])
	{
		if (cmd[i][0] == '&')
		{
			ft_putstr_fd("minishell: syntax error near unexpected"
				" token `&\'\n", 2);
			global_return_int(1, 258);
			return (1);
		}
	}
	return (0);
}

void	execution(t_tree *tree, t_env **myenv, char **env)
{
	char	**cmd;

	if (!tree)
		return ;
	if (tree->node_type == TR_PIPE)
		ft_pipe(tree, myenv, env);
	else if (tree->node_type == TR_REDIR_IN)
		left_redirect(tree, myenv, env);
	else if (tree->node_type == TR_HERE_DOC)
		left_double_redirect(tree, myenv, env);
	else if (tree->node_type == TR_REDIR_OUT)
		right_redirect(tree, myenv, env);
	else if (tree->node_type == TR_REDIR_APND)
		right_double_redirect(tree, myenv, env);
	else if (tree->node_type == TR_COMMAND)
	{
		cmd = (char **)tree->content;
		if (check_and_operator(cmd))
			return ;
		if (is_builtin(cmd[0]))
			run_builtin(cmd, myenv);
		else
			execute_cmd(cmd, myenv);
	}
}
