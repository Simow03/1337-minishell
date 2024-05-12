/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 17:47:26 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/10 23:04:26 by mstaali          ###   ########.fr       */
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

t_env	*sort_env(t_env *var)
{
	int		swapped;
	t_env	*ptr1;
	t_env	*lptr;

	if (var == NULL)
		return (NULL);
	swapped = 0;
	lptr = NULL;
	while (!swapped)
	{
		swapped = 1;
		ptr1 = var;
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
	return (var);
}

void	export(t_env **var, char **cmd)
{
	t_env	*tmp;

	if (!cmd[1])
	{
		tmp = sort_env(*var);
		while (tmp)
		{
			if (ft_strncmp(tmp->name, "_", 1) == 0 && ft_strlen(tmp->name) == 1)
				tmp = tmp->next;
			printf("declare -x %s", tmp->name);
			if (tmp->value)
				printf("=\"%s\"\n", tmp->value);
			tmp = tmp->next;
		}
	}
	else
	{
		parse_args(cmd);
	}
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	t_env    *var;

	var = NULL;
	add_var(env, &var);
	char    *cmd[] = {"export", NULL};
	if (strncmp(cmd[0], "export", 6) == 0)
		export(&var, cmd);
	else
		return (printf("error"), 1);	
}
