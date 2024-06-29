/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 17:53:28 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/21 20:14:17 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_envadd_back(t_env **var, t_env *new)
{
	t_env	*tmp;

	if (!var)
		return ;
	if (*var == NULL)
		*var = new;
	else
	{
		tmp = ft_lstlast(*var);
		tmp->next = new;
	}
}
