/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 21:54:47 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/13 15:59:36 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_token(t_token *token)
{
	t_token *tmp;

	while (token)
	{
		tmp = token->next;
		free(token->content);
		free(token);
		token = tmp;
	}
}

static int	token_size(char *line, t_etoken type)
{
	int		i;
	char	quote;

	if (type == TK_REDIR_APND || type == TK_HERE_DOC)
		return (2);
	if (type == TK_PIPE || type == TK_REDIR_IN || type == TK_REDIR_OUT
		|| type == TK_SPACE)
		return (1);
	i = 0;
	quote = '\0';
	if (line[0] == '\"' || line[0] == '\'')
	{
		quote = line[i++];
		while (line[i] && line[i] != quote)
			i++;
		if (line[i] == quote)
			i++;
		return (i);
	}
	if (line[i] && line[i] == '$')
		i++;
	while (line[i] && line[i] != ' ' && line[i] != '\"' && line[i] != '\''
		&& line[i] != '|' && line[i] != '<' && line[i] != '>' && line[i] != '$')
		i++;
	return (i);
}

static void	token_typing(char *line, t_token *new, t_etoken prev_type)
{
	if (line[0] == '<' && line[1] == '<')
		new->token_type = TK_HERE_DOC;
	else if (line[0] == '>' && line[1] == '>')
		new->token_type = TK_REDIR_APND;
	else if (line[0] == '|')
		new->token_type = TK_PIPE;
	else if (line[0] == '<')
		new->token_type = TK_REDIR_IN;
	else if (line[0] == '>')
		new->token_type = TK_REDIR_OUT;
	else if (line[0] == ' ')
		new->token_type = TK_SPACE;
	else
	{
		new->token_type = TK_COMMAND;
		if (prev_type == TK_HERE_DOC)
			new->token_type = TK_DELIMETER;
		if (prev_type == TK_REDIR_APND || prev_type == TK_REDIR_OUT
			|| prev_type == TK_REDIR_IN)
			new->token_type = TK_REDIR_FILE;
		if (line[0] == '\"')
			new->quote = DOUBLE_Q;
		else if (line[0] == '\'')
			new->quote = SINGLE_Q;
	}
}

static int	get_next_token(char *line, t_token *new, int *error)
{
	int				size;
	int				j;
	static t_etoken	prev_type;

	new->quote = NOT_Q;
	token_typing(line, new, prev_type);
	if (new->token_type != TK_SPACE)
		prev_type = new->token_type;
	size = token_size(line, new->token_type);
	new->content = (char *)malloc(sizeof(char) * (size + 1));
	if (!new->content)
	{
		*error = 1;
		return  (perror("malloc"), 1);
	}
	j = -1;
	while (*line == ' ' && *(line + 1) == ' ')
		line++;
	while (++j < size)
		new->content[j] = *line++;
	new->content[j] = '\0';
	new->next = tokenizer(line, error);
	return (0);
}

t_token	*tokenizer(char *line, int *error)
{
	t_token		*new;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
	{
		perror("malloc");
		*error = 1;
		return (NULL);
	}
	if (!*line)
	{
		free(new);
		return (NULL);
	}
	if (line[0] == '$' && line[1] == '\"')
		line++;
	if (get_next_token(line, new, error))
		return (NULL);
	return (new);
}
