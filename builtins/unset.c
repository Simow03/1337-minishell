/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 20:46:10 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/12 10:34:51 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_first_node(t_env **myenv)
{
	t_env	*tmp;

	if (!myenv || !*myenv)
		return ;
	tmp = *myenv;
	*myenv = (*myenv)->next;
	free(tmp->name);
	free(tmp->value);
	free(tmp);
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
		free(current->name);
		free(current->value);
		free(current);
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
		free(current->name);
		free(current->value);
		free(current);
	}
}

void	process_unset(t_env **myenv, t_env *tmp, t_env *prev, char *cmd)
{
	while (tmp)
	{
		if (strcmp(cmd, "_") == 0)
		{
			tmp = tmp->next;
			continue ;
		}
		if (ft_strcmp(tmp->name, cmd) == 0)
		{
			if (prev == NULL)
				remove_first_node(myenv);
			else if (tmp->next == NULL)
				remove_last_node(myenv);
			else
				remove_mid_node(myenv, cmd);
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void	unset(t_env **myenv, char **cmd)
{
	t_env	*tmp;
	t_env	*prev;
	int		i;

	i = 0;
	while (cmd[++i])
	{
		if (is_valid_name(myenv, cmd[i], "unset"))
		{
			prev = NULL;
			tmp = *myenv;
			process_unset(myenv, tmp, prev, cmd[i]);
			global_return_int(1, 0);
		}
	}
}
