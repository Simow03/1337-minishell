/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:22:15 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/23 18:31:13 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell.h"

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

int main(int ac, char **av, char **en)
{
	t_tree	*tree;
	int		exit_value;

	atexit(f);
	(void)ac;
	(void)av;
	global_env(create_env(en), 1);
	global_return_int(1, 555);
	exit_value = 0;
	int i = 1;
	while (i--)
	{
		tree = NULL;
		tree = parsing();
		if (!tree)
			exit_value = 258;
		printer(tree);
		free_tree(tree);
	}
	free_env(global_env(NULL, 0));
	return (exit_value);
}
