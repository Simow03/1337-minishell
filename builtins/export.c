/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 17:47:26 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/07 17:19:43 by mstaali          ###   ########.fr       */
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

t_env *copy_env(t_env *original)
{
    t_env *new_list = NULL;
    t_env *current = original;
    t_env *tail = NULL;
    while (current)
    {
        t_env *new_node = malloc(sizeof(t_env));
        if (!new_node)
            return (NULL);
        new_node->name = strdup(current->name);
        new_node->value = current->value ? strdup(current->value) : NULL;
        new_node->next = NULL;

        if (!new_list)
            new_list = new_node;
        else
            tail->next = new_node;
        
        tail = new_node;
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

	if (!myenv)
		return (NULL);
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

void	export(t_env **myenv, char **cmd)
{
	t_env	*tmp;
	int		i;

	i = 0;
	if (!cmd[1])
	{
		tmp = sort_env(*myenv);
		while (tmp)
		{
			if (tmp && ft_strcmp(tmp->name, "_"))
			{
				printf("declare -x %s", tmp->name);
				if (tmp->value)
					printf("=\"%s\"\n", tmp->value);
				else
					printf("\n");
			}
			tmp = tmp->next;
		}
	}
	else
		while (cmd[++i])
			process_input(myenv, cmd[i]);
}
