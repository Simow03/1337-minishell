/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_lstnew.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 20:23:44 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/01 17:37:56 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*ft_env_lstnew(char *name, char *value)
{
	t_env *new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->name = ft_strdup(name);
	new->value = ft_strdup(value);
	new->next = NULL;
	return (new);
}
