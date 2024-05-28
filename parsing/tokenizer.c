/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 21:54:47 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/27 15:25:44 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_token(t_tokens *token)
{
	t_tokens	*tmp;

	while (token)
	{
		tmp = token->next;
		free(token->token);
		free(token);
		token = tmp;
	}
}

static int	token_size(char *line)
{
	int	i;
	char	quote;

	i = 0;
	if (line[0] == '<' || line[0] == '>' || line[0] == '|')
	{
		if ((line[0] == '<' && line[1] == '<') || (line[0] == '>' && line[1] == '>'))
			return (2);
		return (1);	
	}
	while (line[i] && line[i] != '<' && line[i] != '>' && line[i] != '|'
		&& line[i] != ' ')
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			quote = line[i++];
			while (line[i] && line[i] != quote)
				i++;
		}
		i++;
	}
	return (i);
}

static int	get_next_token(char *line, t_tokens *new, int *error)
{
	int	i;
	int	j;

	i = token_size(line);
	new->token_type = 0;
	new->is_quoted = 0;
	if (line[0] == '<' || line[0] == '>')
		new->token_type = 1;
	if (line[0] == '|')
		new->token_type = 2;
	new->token = (char *)malloc(sizeof(char) * (i + 1));
	if (!new->token)
	{
		perror("malloc");
		*error = 1;
		return (1);
	}
	j = -1;
	while (++j < i)
		new->token[j] = *line++;
	new->token[j] = '\0';
	new->next = tokenizer(line, error);
	return (0);
}

t_tokens	*tokenizer(char *line, int *error)
{
	t_tokens		*new;

	new = (t_tokens *)malloc(sizeof(t_tokens));
	if (!new)
	{
		perror("malloc");
		*error = 1;
		return (NULL);
	}
	while (*line == ' ')
		line++;
	if (!*line)
	{
		free(new);
		return (NULL);
	}
	if (get_next_token(line, new, error))
		return (NULL);
	return (new);
}
