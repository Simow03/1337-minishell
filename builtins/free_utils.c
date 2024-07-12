/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 11:04:26 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/12 11:06:25 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env(t_env *env)
{
	t_env	*curr;
	t_env	*next;

	curr = env;
	while (curr)
	{
		next = curr->next;
		free(curr->name);
		free(curr->value);
		free(curr);
		curr = next;
	}
}

void	free_input(t_env *input)
{
	free(input->name);
	if (input->value)
		free(input->value);
	free(input);
}

void	free_dbl_str(char **path)
{
	int	j;

	j = 0;
	while (path[j])
		free(path[j++]);
	free(path);
}
