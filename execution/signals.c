/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 13:51:38 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/31 20:10:17 by mstaali          ###   ########.fr       */
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
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		global_return_int(1, 1);
	}
}

void	signal_listener(void)
{
	rl_set_signals();
	signal(SIGINT, main_signo);
	signal(SIGQUIT, SIG_IGN);
}
