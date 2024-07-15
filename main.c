/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:22:15 by ayyassif          #+#    #+#             */
/*   Updated: 2024/07/15 14:16:16 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sigint_received = 0;

void	free_env_list(t_env **myenv)
{
	t_env	*curr;
	t_env	*next;

	if (!myenv || !(*myenv))
		return ;
	curr = *myenv;
	while (curr)
	{
		next = curr->next;
		free(curr->name);
		free(curr->value);
		free(curr);
		curr = next;
	}
	*myenv = NULL;
}

int	main(int ac, char **av, char **env)
{
	t_tree	*tree;
	t_env	*myenv;
	int		exit_value;

	myenv = NULL;
	exit_value = 0;
	add_var(env, &myenv);
	global_return_int(1, 0);
	while (ac && av)
	{
		signal_listener();
		global_env(myenv, 1);
		tree = parsing();
		if (!tree)
			continue ;
		//printer(tree);
		execution(tree, &myenv, env);
		free_tree(tree);
	}
	free_env_list(&myenv);
	return (exit_value);
}
