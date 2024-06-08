/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:22:15 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/08 17:31:30 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t sigint_received = 0;

char    *global_return_str(int mode, int value)
{
    static char    *return_str;
    
    if (mode)
    {
        free(return_str);
        return_str = NULL;
        if (mode == 1)
            return_str = ft_itoa(value);
    }
    return (return_str);
}

int	global_return_int(int mode, int value)
{
	static int	return_value;

	if (mode)
	{
		global_return_str(1, value);
		return_value = value;
	}
	return (return_value);
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
	while (1)
	{
		signal_listener();
		tree = NULL;
		tree = parsing(myenv);
		if (!tree)
			exit_value = 258;
		execution(tree, &myenv, env);
		free_tree(tree);
	}
	free_env(myenv);
	return (exit_value);
}
