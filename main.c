/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:22:15 by ayyassif          #+#    #+#             */
/*   Updated: 2024/07/01 14:25:02 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t sigint_received = 0;

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

	(void)ac;
	(void)av;
	myenv = NULL;
	add_var(env, &myenv);
	global_return_int(1, 0);
	while (1)
	{
		signal_listener();
		global_env(myenv, 1);
		tree = parsing();
		if (!tree)
			exit_value = 258;
		execution(tree, &myenv, env);
		free_tree(tree);
	}
	free_env_list(&myenv);
	return (exit_value);
}
