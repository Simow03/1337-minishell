/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 22:55:41 by mstaali           #+#    #+#             */
/*   Updated: 2024/05/21 20:32:03 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	split_cmd(char *s, t_env *input)
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	input->name = ft_substr(s, 0, i);
	if (s[i] == '=')
		input->value = ft_substr(s, i + 1, ft_strlen(s) - i - 1);
	else
		input->value = NULL;
}

static int	is_valid_name(t_env **myenv, char *name)
{
	char	**cmd;
	int		i;

	i = 0;
	cmd = (char *[]){"export", NULL};
	if (name[i] == '#')
	{
		export(myenv, cmd);
		return (0);
	}
	while (name[i])
	{
		if ((i == 0 && !ft_isalpha(name[i]) && name[i] != '_')
			|| (!ft_isdigit(name[i]) && !ft_isalpha(name[i]) && name[i] != '_'))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(name, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			//! exit_status is 1
			return (0);
		}
		i++;
	}
	return (1);
}

void	process_input(t_env **myenv, char *cmd)
{
	t_env	*input;
	t_env	*tmp;

	input = malloc(sizeof(t_env));
	input->next = NULL;
	if (!input)
		return ;
	split_cmd(cmd, input);
	tmp = *myenv;
	if (is_valid_name(myenv, input->name))
	{
		while (tmp)
		{
			if (ft_strcmp(tmp->name, input->name) == 0 && input->value != NULL)
			{
				tmp->value = input->value;
				break ;
			}
			else if (ft_strcmp(tmp->name, input->name) == 0)
				break ;
			tmp = tmp->next;
		}
		if (!tmp)
			ft_envadd_back(myenv, input);
	}
}
