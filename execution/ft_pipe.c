/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 01:25:36 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/12 09:08:15 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	left_tree(t_tree *tree, int fd[2], t_env **myenv, char **env)
{
	int	status;

	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	execution(tree->left, myenv, env);
	status = global_return_int(0, 0);
	exit(status);
}

static void	right_tree(t_tree *tree, int fd[2], t_env **myenv, char **env)
{
	int	status;

	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	close(fd[0]);
	execution(tree->right, myenv, env);
	status = global_return_int(0, 0);
	exit(status);
}

void	ft_pipe(t_tree *tree, t_env **myenv, char **env)
{
	int	fd[2];
	int	pid1;
	int	pid2;
	int	status1;
	int	status2;

	if (pipe(fd) < 0)
		exit(1);
	pid1 = fork();
	if (pid1 < 0)
		error_fork();
	if (pid1 == 0)
		left_tree(tree, fd, myenv, env);
	pid2 = fork();
	if (pid2 < 0)
		error_fork();
	if (pid2 == 0)
		right_tree(tree, fd, myenv, env);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	get_status(status2);
}
