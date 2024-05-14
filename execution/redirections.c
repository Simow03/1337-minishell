/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:59:00 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/13 21:20:25 by mstaali          ###   ########.fr       */
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
		//TODO: create an error message
		perror(file);
		close(init_fd);
		//! exit_status is = 1
	}
	dup2(fd, STDIN_FILENO);
	execution(tree->left, myenv, env);
	dup2(init_fd, STDIN_FILENO);
	close(init_fd);
	close(fd);
}

// int	left_double_redirect(t_tree *tree, t_env **myenv, char **env)
// {
	
// }

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
		//TODO: create an error message
		perror(file);
		close(init_fd);
		//! exit_status is = 1
	}
	dup2(fd, STDOUT_FILENO);
	execution(tree->left, myenv, env);
	dup2(init_fd, STDOUT_FILENO);
	close(init_fd);
	close(fd);
}

// int	right_double_redirect(t_tree *tree, t_env **myenv, char **env)
// {
	
// }
