/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 19:28:04 by mstaali           #+#    #+#             */
/*   Updated: 2024/06/07 20:12:48 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_current_dir(void)
{
	char	**curr_path;
	char	*curr_dir;
	int		i;

	curr_dir = NULL;
	curr_path = ft_split(getcwd(NULL, 0), '/');
	i = 0;
	while (curr_path[i])
		i++;
	if (i >= 1)
		curr_dir = ft_strdup(curr_path[i - 1]);
	i = 0;
	while (curr_path[i])
		free(curr_path[i++]);
	free(curr_path);
	if (!curr_dir)
		return(strdup("/"));
	return (curr_dir);
}

char	*init_prompt(void)
{
	char	*prompt;
	char	*curr_dir;

	prompt = ft_strdup("\001\033[1;92m\002");
	prompt = ft_strjoin(prompt, "➜  \001\033[1;94m\002minishell:\001\033[1;94m\002(\001\033[1;91m\002");
	curr_dir = get_current_dir();
	prompt = ft_strjoin(prompt, curr_dir);
	free(curr_dir);
	prompt = ft_strjoin(prompt, "\001\033[1;94m\002) $ \001\033[0;39m\002");
	return (prompt);
}
