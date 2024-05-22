/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:36:42 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/22 16:09:59 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void	hrdc_tree(t_tree *new, t_tokens *token)
{
	new->node_type = 2;
	free(new->content);
	new->content = (void *)here_doc_handler(token->next->token,
		global_env(NULL, 0), token->next->is_quoted);
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
		return (NULL);
	i = -1;
	while (++i < len1 - 1)
		str[i] = s1[i];
	if (s1)
		str[i++] = '\n';
	i = -1;
	while (++i < len2)
		str[len1 + i] = s2[i];
	str[len1 + i] = '\0';
	return (str);
}

int	delicmp(char *line, char *deli)
{
	int		i;
	int		j;
	char	quote;

	i = -1;
	j = 0;
	quote = '\0';
	while (deli[++i])
	{
		if (deli[i] == '$' && deli[i+1] == '$')
		{
			i++;
			if (line[j++] != '$' && line[j++] != '$')
				break ;
		}
		else if (!quote && deli[i] == '$' && (deli[i + 1] == '\'' || deli[i + 1] == '\"'))
			continue ;
		if (!quote && (deli[i] == '\'' || deli[i] == '\"'))
			quote = deli[i];
		else if (quote == deli[i])
			quote = '\0';
		else if (deli[i] != line[j++])
			break;
	}
	return (deli[i] - line[j]);
}

char	*here_doc_expand(char *text)
{
	int		i;
	int		size;
	char	*new;

	i = -1;
	size = 0;
	while (text[++i])
		if (text[i] == '$')
			value_fetcher(&text[i + 1], global_env(NULL, 0), &size);
	new = (char *)malloc(sizeof(char) * (i + size + 1));
	if (!new)
		return (perror("malloc"), NULL);
	i = 0;
	while (*text)
	{
		if (*text == '$')
			text += get_next_expand(text + 1, new, &i);
		else
			new[i++] = *text;
		if (!(*(text++)))
			break;
	}
	new[i] = '\0';
	return (new);
}

char	*here_doc_handler(char	*delimeter, t_env *env, int is_quoted)
{
	char	*line;
	char	*text;
	char	*tmp;
	int		check;

	(void)is_quoted;
	check = 1;
	text = NULL;
	while (check)
	{
		line = readline("> ");
		rl_on_new_line();
		if (!line)
			break;
		check = delicmp(line, delimeter);
		tmp = ft_hrdc_join(text, line, check);
		free(text);
		free(line);
		if (!tmp)
			return (perror("malloc"), NULL);
		text = tmp;
	}
	if (is_quoted || !env)
		return(text);
	return(here_doc_expand(text));
}
