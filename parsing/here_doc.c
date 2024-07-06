/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:36:42 by ayyassif          #+#    #+#             */
/*   Updated: 2024/07/06 20:13:02 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	error_hrdc(t_token *token, int pos)
{
	pos--;
	while (token && token->next && --pos)
	{
		if (token->token_type == TK_HERE_DOC)
			free(here_doc_handler(token->next));
		token = token->next;
	}
}

static char	*ft_hrdc_join(char *s1, char *s2, int check)
{
	int		i;
	int		len1;
	int		len2;
	char	*str;

	if (!check)
		s2 = "\0";
	len1 = 0;
	if (s1)
		len1 = ft_strlen(s1) + 1;
	len2 = ft_strlen(s2);
	str = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!str)
		return (free(s1), NULL);
	i = -1;
	while (++i < len1 - 1)
		str[i] = s1[i];
	if (s1)
		str[i++] = '\n';
	i = -1;
	while (++i < len2)
		str[len1 + i] = s2[i];
	str[len1 + i] = '\0';
	return (free(s1), str);
}

int	new_deli_size(t_token *token)
{
	int	size;

	size = ft_strlen(token->content);
	if (token->quote != NOT_Q)
		size -= 2;
	if (token->next && token->next->token_type == TK_DELIMETER)
		return (size + new_deli_size(token->next));
	return (size);
}

char	*new_delimeter(t_token *token, int *is_quote)
{
	char	*new_deli;
	char	*deli;
	int		j;
	int		i;

	new_deli = malloc(sizeof(char) * (new_deli_size(token) + 1));
	j = 0;
	while (token && token->token_type == TK_DELIMETER)
	{
		i = -1;
		deli = token->content;
		while (deli[++i])
		{
			if (token->quote == NOT_Q || (token->quote == DOUBLE_Q && deli[i] != '\"')
				|| (token->quote == SINGLE_Q && deli[i] != '\''))
				new_deli[j++] = deli[i];
			else
				*is_quote = 1;
		}
		token = token->next;
	}
	new_deli[j] = '\0';
	return (new_deli);
}

t_token	*return_hrdc(char *deli, t_token *token, t_token *returned, char *text)
{
	t_token	*tmp;

	free(text);
	free(deli);
	if (!returned)
	{
		perror("malloc");
		free_token(returned);
		return (NULL);
	}
	while (token && token->token_type == TK_DELIMETER)
	{
		tmp = token->next;
		free(token->content);
		free(token);
		token = tmp;
	}
	tmp = returned;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = token;
	return (returned);
}

t_token	*here_doc_handler(t_token *token)
{
	char	*line;
	char	*text;
	char	*deli;
	int		check;
	int 	is_quote;

	check = 1;
	text = NULL;
	is_quote = 0;
	if (token->token_type == TK_SPACE)
		deli = new_delimeter(token->next, &is_quote);
	else
		deli = new_delimeter(token, &is_quote);
	while (check)
	{
		signal_listener();
		line = readline("> ");
		if (!line)
		{
			if (text)
				text = ft_hrdc_join(text, "\0", check);
			break;
		}
		check = ft_strcmp(line, deli);
		text = ft_hrdc_join(text, line, check);
		free(line);
		if (!text)
			return (return_hrdc(deli, NULL, NULL, NULL));
	}
	return(return_hrdc(deli, token, here_doc_expand(text, is_quote), text));
}
