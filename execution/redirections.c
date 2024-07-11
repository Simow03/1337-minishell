/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:59:00 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/11 14:06:06 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	left_redirect(t_tree *tree, t_env **myenv, char **env)
{
	int		init_fd;
	int		fd;
	char	*file;

	init_fd = dup(STDIN_FILENO);
	file = (char *)tree->content;
	fd = open(file, O_RDONLY, 0644);
	if (fd < 0)
	{
		perror(file);
		global_return_int(1, 1);
		close(init_fd);
		return ;
	}
	dup2(fd, STDIN_FILENO);
	execution(tree->left, myenv, env);
	dup2(init_fd, STDIN_FILENO);
	close(init_fd);
	close(fd);
}

void	left_double_redirect(t_tree *tree, t_env **myenv, char **env)
{
	int		init_fd;
	int		fd[2];
	char	*here_doc;

	init_fd = dup(STDIN_FILENO);
	here_doc = (char *)tree->content;
	if (pipe(fd) < 0)
	{
		perror("pipe");
		global_return_int(1, 1);
		return ;
	}
	write(fd[1], here_doc, ft_strlen(here_doc));
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	execution(tree->left, myenv, env);
	dup2(init_fd, STDIN_FILENO);
	close(init_fd);
}

void	right_redirect(t_tree *tree, t_env **myenv, char **env)
{
	int		init_fd;
	int		fd;
	char	*file;

	init_fd = dup(STDOUT_FILENO);
	file = (char *)tree->content;
	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(file);
		global_return_int(1, 1);
		close(init_fd);
		return ;
	}
	dup2(fd, STDOUT_FILENO);
	execution(tree->left, myenv, env);
	dup2(init_fd, STDOUT_FILENO);
	close(init_fd);
	close(fd);
}

void	right_double_redirect(t_tree *tree, t_env **myenv, char **env)
{
	int		init_fd;
	int		fd;
	char	*file;

	init_fd = dup(STDOUT_FILENO);
	file = (char *)tree->content;
	fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(file);
		close(init_fd);
		global_return_int(1, 1);
		return ;
	}
	dup2(fd, STDOUT_FILENO);
	execution(tree->left, myenv, env);
	dup2(init_fd, STDOUT_FILENO);
	close(init_fd);
	close(fd);
}
