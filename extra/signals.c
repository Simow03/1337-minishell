/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 13:51:38 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/06 20:13:45 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sigint_handler(int signo)
{
	if (signo == SIGINT)
	{
		sigint_received = 1;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		global_return_int(1, 1);
	}
}


void	signal_listener(void)
{
	rl_set_signals();
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
