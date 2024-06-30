/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:22:15 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/30 14:47:29 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t sigint_received = 0;

void	f()
{
	system("leaks minishell");
}

void	printer(t_tree *tree)
{
	t_tree		*branch;

	while (tree)
	{
		branch = tree;
		while (branch)
		{
			if (branch->node_type == TR_COMMAND)
			{
				int i = -1;
				while (((char **)(branch->content))[++i])
					printf("string: (%s)\n", ((char **)(branch->content))[i]);
			}
			else
				printf("%d\t(%s)\n", branch->node_type, ((char *)branch->content));
			branch = branch->left;
		}
		puts("============================");
		tree = tree->right;
	}
}

int main(int ac, char **av, char **env)
{
	t_tree	*tree;
	t_env	*myenv;
	int		exit_value;

	//atexit(f);
	(void)ac;
	(void)av;
	myenv = create_env(env);
	global_return_int(1, 0);
	exit_value = 0;
	int i = 1;
	while (i)
	{
		//signal_listener();
		global_env(myenv, 1);
		tree = parsing();
		if (!tree)
			exit_value = 258;
		printer(tree);
		//execution(tree, &myenv, env);
		free_tree(tree);
	}
	free_env(myenv);
	return (exit_value);
}
