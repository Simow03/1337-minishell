/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:36:42 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/07 17:14:49 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ft_hrdc_join(char *s1, char *s2)
{
	int		i;
	int		len1;
	int		len2;
	char	*str;

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

int	hrdoccmp(char *line, char *deli)
{
	int		i;
	int		j;
	char	quote;

	i = -1;
	j = 0;
	quote = '\0';
	while (deli[++i])
	{
		if (sigint_received)
			return (-1);
		if (!quote && deli[i] == '$' && (deli[i + 1] == '\'' || deli[i + 1] == '\"'))
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

char	*here_doc_handler(char *delimeter, int is_quoted)
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
		signal_listener();
		line = readline("> ");
		rl_on_new_line();
		if (!line)
			break;
		check = hrdoccmp(line, delimeter);
		if (check == -1)
			break;
		if (!check)
			tmp = ft_hrdc_join(text, "\0");
		else
			tmp = ft_hrdc_join(text, line);
		free(text);
		free(line);
		if (!tmp)
			return (perror("malloc"), NULL);
		text = tmp;
	}
	sigint_received = 0;
	return (text);
}
