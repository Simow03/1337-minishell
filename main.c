/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:22:15 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/11 17:23:32 by ayyassif         ###   ########.fr       */
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
			if (!branch->node_type)
			{
				int i = -1;
				while (((char **)(branch->content))[++i])
				{
					printf("string: %s\n", ((char **)(branch->content))[i]);
				}
			}
			else
				printf("%d\t%s\n", branch->node_type, (char *)branch->content);
			branch = branch->left;
		}
		puts("--------");
		tree = tree->right;
	}
}

int main(int ac, char **av, char **en)
{
	t_tree	*tree;
	t_env	*myenv;
	int		exit_value;

	(void)ac;
	(void)av;
	myenv = NULL;
	add_var(en, &myenv);
	exit_value = 0;
	while (1)
	{
		tree = NULL;
		tree = parsing(myenv);
		if (!tree)
			exit_value = 258;
		printer(tree);
		free_tree(tree);
	}
	free_env(myenv);
	return (exit_value);
}
