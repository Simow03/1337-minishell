/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 13:51:38 by mstaali           #+#    #+#             */
/*   Updated: 2024/06/08 17:32:52 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sigint_handler(int signo)
{
	if (signo == SIGINT)
	{
		sigint_received = 1;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		global_return_int(1, 1);
	}
}

void	signal_listener()
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	sigint_received = 0;
}
