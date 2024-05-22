/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 20:46:10 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/22 12:27:09 by mstaali          ###   ########.fr       */
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
	//TODO: tmp needs to be free'd
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
		//TODO: current needs to be free'd
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
		//TODO: current needs to be free'd
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
