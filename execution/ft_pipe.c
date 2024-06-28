/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 01:25:36 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/13 21:17:34 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	left_tree(t_tree *tree, int fd[2], t_env **myenv, char **env)
{
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	execution(tree->left, myenv, env);
	exit(0);
}

static void	right_tree(t_tree *tree, int fd[2], t_env **myenv, char **env)
{
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	close(fd[0]);
	execution(tree->right, myenv, env);
	exit(0);
}

void	ft_pipe(t_tree *tree, t_env **myenv, char **env)
{
	int	fd[2];
	int	pid1;
	int	pid2;

	if (pipe(fd) < 0)
		exit(1);
	pid1 = fork();
	if (pid1 < 0)
		error_fork();
	if (pid1 == 0)
		left_tree(tree,fd, myenv, env);
	pid2 = fork();
	if (pid2 < 0)
		error_fork();
	if (pid2 == 0)
		right_tree(tree, fd, myenv, env);
	close(fd[0]);
	close(fd[1]);
	//TODO: NULL needs to be replaced with &exit_status
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}
