/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 22:55:41 by mstaali           #+#    #+#             */
/*   Updated: 2024/07/09 18:35:11 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	identifier_error(char *name, char *flag)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(flag, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	global_return_int(1, 1);
	return (0);
}

int	is_valid_name(t_env **myenv, char *name, char *flag)
{
	char	**cmd;
	int		i;

	i = 0;
	cmd = (char *[]){"export", NULL};
	if (name[i] == '#')
	{
		if (ft_strcmp(flag, "export") == 0)
			export(myenv, cmd);
		return (0);
	}
	while (name[i])
	{
		if ((i == 0 && !ft_isalpha(name[i]) && name[i] != '_')
			|| (!ft_isdigit(name[i]) && !ft_isalpha(name[i]) && name[i] != '_')
			|| (name[i] == '+' && (name[i + 1] == '+' || name[i] == '\0')))
				return(identifier_error(name, flag));
		i++;
	}
	return (1);
}

static int	split_cmd(t_env **myenv, char *s, t_env *input)
{
	int	i;
	int	concat_flag;

	if (!s)
		return (0);
	i = 0;
	concat_flag = 0;
	if (s[0] == '+' || s[0] == '=')
		return (identifier_error(s, "export"), -1);
	while (s[i] && s[i] != '=' && s[i] != '+')
		i++;
	if (s[i] == '+' && s[i + 1] != '=')
		return (identifier_error(s, "export"), -1);
	if (s[i] == '+' && s[i + 1] == '=')
	{
		concat_flag = 1;
		input->name = ft_substr(s, 0, i);
		i++;
	}
	else
	{
		while (s[i] && s[i] != '=')
			i++;
		input->name = ft_substr(s, 0, i);
	}
	if (s[i] == '=')
		input->value = ft_substr(s, i + 1, ft_strlen(s) - i - 1);
	else
		input->value = NULL;
	if (is_valid_name(myenv, input->name, "export"))
		return (concat_flag);
	else
	{
		free(input->name);
		free(input->value);
		return (-1);
	}
}


char	*concat_value(char *s1, const char *s2)
{
	char	*buffer;
	int		i;
	int		j;

	buffer = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!buffer)
		return (NULL);
	i = 0;
	j = 0;
	if (s1)
		while (s1[i])
			buffer[j++] = s1[i++];
	i = 0;
	if (s2)
		while (s2[i])
			buffer[j++] = s2[i++];
	buffer[j] = '\0';
	return (buffer);
}

void	process_input(t_env **myenv, char *cmd)
{
	t_env	*input;
	t_env	*tmp;
	char	*old_value;
	int		concat_flag;

	input = malloc(sizeof(t_env));
	if (!input)
		return ;
	input->next = NULL;
	concat_flag = split_cmd(myenv, cmd, input);
	if (concat_flag == -1)
		return (free(input));
	tmp = *myenv;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, input->name) == 0 && input->value != NULL)
		{
			if (concat_flag)
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
	global_return_int(1, 0);
}
