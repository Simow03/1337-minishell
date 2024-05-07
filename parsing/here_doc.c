/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:36:42 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/07 16:30:34 by ayyassif         ###   ########.fr       */
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

char	*here_doc_handler(char	*delimeter)
{
	char	*line;
	char	*text;
	char	*tmp;
	int		check;

	check = 1;
	text = NULL;
	while (check)
	{
		line = readline("> ");
		rl_on_new_line();
		if (!line)
			break;
		check = ft_strncmp(line, delimeter, ft_strlen(delimeter));
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
	return(text);
}