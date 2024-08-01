/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 13:51:38 by mstaali           #+#    #+#             */
/*   Updated: 2024/08/01 15:08:22 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_status(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
		else if (WTERMSIG(status) == SIGINT)
			printf("\n");
		return (global_return_int(1, 128 + status));
	}
	return (global_return_int(1, WEXITSTATUS(status)));
}

void	exec_signo(int signo)
{
	if (signo == SIGINT || signo == SIGQUIT)
		g_sigint_received = 1;
}

void	main_signo(int signo)
{
	if (signo == SIGINT)
	{
		g_sigint_received = 1;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		global_return_int(1, 1);
	}
}

void	signal_listener(void)
{
	signal(SIGINT, main_signo);
	signal(SIGQUIT, SIG_IGN);
}
