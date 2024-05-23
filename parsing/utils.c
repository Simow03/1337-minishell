/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 23:20:58 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/23 12:40:05 by ayyassif         ###   ########.fr       */
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

// setter: mode = 1
// getter: mode = 0

int	global_return_int(int mode, int value)
{
	static int	return_value;

	if (mode)
		global_return_str(1, value);
		return_value = value;
	return (return_value);
}

// free:	mode = 2
// setter:	mode = 1
// getter:	mode = 0

char	*global_return_str(int mode, int value)
{
	static char	*return_str;
	
	if (mode)
	{
		free(return_str);
		return_str = NULL;
		if (mode == 1)
			return_str = ft_itoa(value);
	}
	return (return_str);
}