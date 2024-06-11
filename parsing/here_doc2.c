/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 18:41:46 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/02 15:54:16 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	int		size;

	size = ft_strlen(token->content);
	if (token->quote != NOT_Q)
		size -= 2;
	if (token->next->token_type == TK_DELIMETER)
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
	i = -1;
	j = -1;
	while (token->token_type == TK_DELIMETER)
	{
		deli = token->content;
		while (deli[++i])
		{
			if (!(token->quote == DOUBLE_Q && deli[i] == '\"')
				|| !(token->quote == SINGLE_Q && deli[i] == '\''))
				new_deli[++j] = deli[i];
			else
				*is_quote = 1;
		}
		token = token->next;
	}
	return (new_deli);
}

char	*here_doc_handler2(t_token *token, t_env *env)
{
	char	*line;
	char	*text;
	char	*deli;
	int		check;
	int 	is_quote;

	check = 1;
	text = NULL;
	is_quote = 0;
	deli = new_delimeter(token, &is_quote);
	while (check)
	{
		line = readline("> ");
		rl_on_new_line();
		if (!line)
			break;
		check = ft_strcmp(line, token);
		text = ft_hrdc_join(text, line, check);
		free(line);
		if (!text)
			return (perror("malloc"), NULL);
	}
	if (is_quote || !env)
		return(text);
	return(here_doc_expand(text));
}
