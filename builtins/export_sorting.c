/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_sorting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:43:20 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/12 10:44:24 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	swap(t_env *a, t_env *b)
{
	char	*tmp_name;
	char	*tmp_value;

	tmp_name = a->name;
	tmp_value = a->value;
	a->name = b->name;
	a->value = b->value;
	b->name = tmp_name;
	b->value = tmp_value;
}

t_env	*copy_env(t_env *original)
{
	t_env	*new_list;
	t_env	*current;
	t_env	*last;
	t_env	*new_node;

	new_list = NULL;
	last = NULL;
	current = original;
	while (current)
	{
		new_node = ft_env_lstnew(current->name, current->value);
		if (!new_list)
			new_list = new_node;
		else
			last->next = new_node;
		last = new_node;
		current = current->next;
	}
	return (new_list);
}

t_env	*sort_env(t_env *myenv)
{
	int		swapped;
	t_env	*ptr1;
	t_env	*lptr;
	t_env	*copy;

	copy = copy_env(myenv);
	swapped = 0;
	lptr = NULL;
	while (!swapped)
	{
		swapped = 1;
		ptr1 = copy;
		while (ptr1->next != lptr)
		{
			if (ft_strcmp(ptr1->name, ptr1->next->name) > 0)
			{
				swap(ptr1, ptr1->next);
				swapped = 0;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	}
	return (copy);
}
