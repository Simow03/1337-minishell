/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 17:47:26 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/12 11:57:17 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_export(t_env *tmp)
{
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

void	change_value(t_env *tmp, t_env *input, int *concat_flag)
{
	char	*old_value;

	if (*concat_flag)
	{
		old_value = tmp->value;
		tmp->value = concat_value(tmp->value, input->value);
		free(old_value);
		free(input->name);
		free(input->value);
		free(input);
	}
	else
	{
		free(tmp->value);
		tmp->value = ft_strdup(input->value);
		free(input->name);
		free(input->value);
		free(input);
	}
}

void	check_input(t_env **myenv, t_env *tmp, t_env *input, int *concat_flag)
{
	while (tmp)
	{
		if (ft_strcmp(tmp->name, input->name) == 0 && input->value != NULL)
		{
			change_value(tmp, input, concat_flag);
			break ;
		}
		else if (ft_strcmp(tmp->name, input->name) == 0)
		{
			free(input->name);
			if (input->value)
				free(input->value);
			free(input);
			break ;
		}
		tmp = tmp->next;
	}
	if (!tmp)
		ft_envadd_back(myenv, input);
}

void	process_input(t_env **myenv, char *cmd)
{
	t_env	*input;
	t_env	*tmp;
	int		concat_flag;

	input = malloc(sizeof(t_env));
	if (!input)
		return ;
	input->next = NULL;
	concat_flag = split_cmd(myenv, cmd, input);
	if (concat_flag == -1)
		return (free(input));
	tmp = *myenv;
	check_input(myenv, tmp, input, &concat_flag);
	global_return_int(1, 0);
}

void	export(t_env **myenv, char **cmd)
{
	t_env	*tmp;
	t_env	*sorted_env;
	int		i;

	i = 0;
	if (!cmd[1])
	{
		sorted_env = sort_env(*myenv);
		if (sorted_env)
		{
			tmp = sorted_env;
			print_export(tmp);
			free_env(sorted_env);
			global_return_int(1, 0);
		}
	}
	else
		while (cmd[++i])
			process_input(myenv, cmd[i]);
}
