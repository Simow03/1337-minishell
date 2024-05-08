/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:22:15 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/08 18:24:35 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int main(int ac, char **av, char **env)
{
	// t_tree	*tree;
	t_env	*myenv;
	//int		exit_value;

	(void)ac;
	(void)av;
	//atexit(f);
	myenv = create_env(env);
	free_env(myenv);
	// while (myenv)
	// {
	// 	printf("%s=%s\n", myenv->name, myenv->value);
	// 	myenv = myenv->next;
	// }
	// while (1)
	// {
	// 	tree = NULL;
	// 	tree = parsing();
	// 	printer(tree);
		
	// 	free_tree(tree);
	// }
	return (EXIT_SUCCESS);
}
