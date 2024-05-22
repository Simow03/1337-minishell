/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 23:20:58 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/22 15:57:12 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// setter: mode = 1
// getter: mode = 0

t_env	*global_env(t_env *env, int mode)
{
	static t_env	*global_env;
	if (mode)
		global_env = env;
	return (global_env);
}