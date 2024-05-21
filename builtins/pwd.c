/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 22:44:34 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/21 20:32:14 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pwd(void)
{
	char	*buffer;

	buffer = getcwd(NULL, 0);
	if (!buffer)
		ft_putstr_fd(getenv("PWD"), 1);
	else
	{
		ft_putstr_fd(buffer, 1);
		free(buffer);
	}
	ft_putstr_fd("\n", 1);
}
