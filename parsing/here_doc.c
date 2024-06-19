/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:36:42 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/19 17:01:44 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



// char	*here_doc_expand(char *text)
// {
// 	int		i;
// 	int		size;
// 	char	*new;

// 	i = -1;
// 	size = 0;
// 	while (text[++i])
// 		if (text[i] == '$')
// 			value_fetcher(&text[i + 1], global_env(NULL, 0), &size);
// 	new = (char *)malloc(sizeof(char) * (i + size + 1));
// 	if (!new)
// 		return (perror("malloc"), NULL);
// 	i = 0;
// 	while (*text)
// 	{
// 		if (*text == '$')
// 			text += get_next_expand(text + 1, new, &i);
// 		else
// 			new[i++] = *text;
// 		if (!(*(text++)))
// 			break;
// 	}
// 	new[i] = '\0';
// 	return (new);
// }

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
	printf("size: %d\n", new_deli_size(token));
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

char	*here_doc_handler(t_token *token, t_env *env)
{
	char	*line;
	char	*text;
	char	*deli;
	int		check;
	int 	is_quote;

	(void)env;
	check = 1;
	text = NULL;
	is_quote = 0;
	if (token->token_type == TK_SPACE)
		token = token->next;
	deli = new_delimeter(token, &is_quote);
	while (check)
	{
		line = readline("> ");
		rl_on_new_line();
		if (!line)
			break;
		check = ft_strcmp(line, deli);
		text = ft_hrdc_join(text, line, check);
		free(line);
		if (!text)
			return (perror("malloc"), NULL);
	}
	//if (is_quote || !env)
	return(text);
	//return(here_doc_expand(text));
}
