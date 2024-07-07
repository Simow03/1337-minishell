/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 20:46:10 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/07 17:18:03 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_first_node(t_env **myenv)
{
	t_env	*tmp;
	
	if(!myenv || !*myenv)
		return ;
	tmp = *myenv;
	*myenv = (*myenv)->next;
}

void	remove_last_node(t_env **myenv)
{
	t_env	*prev;
	t_env	*current;

	if (!myenv || !*myenv)
		return ;
	prev = NULL;
	current = *myenv;
	while (current->next)
	{
		prev = current;
		current = current->next;
	}
	if (prev)
	{
		prev->next = NULL;
	}
	else
		remove_first_node(myenv);
}

void	remove_mid_node(t_env **myenv, char *name)
{
	t_env	*prev;
	t_env	*current;

	if (!myenv || !*myenv || !name)
		return ;
	prev = NULL;
	current = *myenv;
	while (current && ft_strcmp(current->name, name) != 0)
	{
		prev = current;
		current = current->next;
	}
	if (current)
	{
		prev->next = current->next;
	}
}

void	unset(t_env **myenv, char **cmd)
{
	t_env	*tmp;
	int		i;

	i = 0;
	while (cmd[++i])
	{
		if (is_valid_name(myenv, cmd[i], "unset"))
		{
			tmp = *myenv;
			while (tmp)
			{
				if (strcmp(cmd[i], "_") == 0)
				{
					tmp = tmp->next;
					continue ;
				}
				if (ft_strcmp(tmp->name, cmd[i]) == 0)
				{
					if (tmp == *myenv)
						remove_first_node(myenv);
					else if (tmp->next == NULL)
						remove_last_node(myenv);
					else
						remove_mid_node(myenv, cmd[i]);
				}
				tmp = tmp->next;
			}
		}
	}
}
