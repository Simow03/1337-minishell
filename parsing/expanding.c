/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:20:57 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/22 16:48:53 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	expand_init(int *is_ambiguous, int *i, int *j, char *quote)
{
	*is_ambiguous = 1;
	*i = 0;
	*j = 0;
	*quote = '\0';
	return (0);
}

//	this function exports the expanded string
//	and add normal charcters to result

char	*expanding_tools(int *is_ambiguous, char *result, int type, char *text)
{
	char	*err_msg;

	if (type == -1)
	{
		*is_ambiguous = ambiguous(NULL, '\0', *is_ambiguous);
		*result = *text;
		return (NULL);
	}
	if ((type == 3 || type == 8) && *is_ambiguous)
	{
		err_msg = error_printer(3, text);
		ft_putstr_fd(err_msg, STDERR_FILENO);
		free(err_msg);
		free(result);
		return (NULL);
	}
	return (result);
}


int	quoting(int *is_ambiguous, char text, char *quote)
{
	if (!(*quote) && (text == '\"' || text == '\''))
	{
		*is_ambiguous = ambiguous(NULL, *quote, *is_ambiguous);
		*quote = text;
		return (1);
	}
	else if (*quote == text)
	{
		*quote = '\0';
		return (1);
	}
	return (0);
}

// 0 : no
// 1 : yes
// 2 : definitely

char	*expanding(char *text, int type)
{
	char	*result;
	char	quote;
	int		i;
	int		j;
	int		is_ambiguous;

	result = (char *)malloc(sizeof(char) * (sizeofexpndng(text) + 1));
	if (!result || expand_init(&is_ambiguous, &i, &j, &quote))
		return (NULL);
	while (text[j])
	{
		if (quote != '\'' && text[j] == '$')
		{
			if ((text[++j] == '\"' || text[j] == '\'') && !quote)
				continue ;
			is_ambiguous = ambiguous(&text[j], quote, is_ambiguous);
			j += get_next_expand(&text[j], result, &i);
			continue ;
		}
		else if (!quoting(&is_ambiguous, text[j], &quote))
			expanding_tools(&is_ambiguous, &result[i++], -1, &text[j]);
		j++;
	}
	result[i] = '\0';
	return (expanding_tools(&is_ambiguous, result, type, text));
}
