/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 21:54:51 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/11 10:11:03 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int	arg_size(char *str)
// {
// 	int	i;
// 	int	size;
// 	char	*value;

// 	i = -1;
// 	size = 0;
// 	while (str[++i])
// 		if (str[i] == '$')
// 		{
// 			value = value_fetcher(&str[i + 1], global_env(NULL, 0), NULL);
// 			while (value && *value)
// 			{
// 				while (*value == ' ')
// 					value++;
// 				size++;
// 				while (*value && *value != ' ')
// 					value++;
// 			}
// 		}
// 	if (!size)
// 		return (1);
// 	return (size);
// }



static char	*reading_line(void)
{
	char	*line;

	line = NULL;
	line = readline("my bash$ ");
	rl_on_new_line();
	if (line && *line)
	{
		add_history(line);
		return (line);
	}
	return (free(line), NULL);
}

// void	free_tree(t_tree *tree)
// {
// 	int	i;

// 	if (!tree)
// 		return ;
// 	free_tree(tree->left);
// 	free_tree(tree->right);
// 	i = -1;
// 	if (!tree->node_type)
// 		while (((char **)tree->content)[++i])
// 			free(((char **)tree->content)[i]);
// 	free(tree->content);
// 	free(tree);
// }

void	error_hrdc(t_token *token, int pos)
{
	pos--;
	while (token && token->next && --pos)
	{
		if (token->token_type == TK_HERE_DOC)
			free(here_doc_handler(token->next, NULL));
		token = token->next;
	}
}

// mode 0: setter
// mode 1: getter


// static void token_printer(t_token *token)
// {
// 	while (token)
// 	{
// 		if (token->content)
// 			printf("%d\t%s\n", token->token_type, token->content);
// 		token = token->next;
// 	}
// }

t_tree	*parsing()
{
	int			error;
	char		*line;
	t_token		*token;
	//t_tree		*tree;

	line = reading_line();
	if (!line)
		return (NULL);
	error = 0;
	token = tokenizer(line, &error);
	free(line);
	if (error)
		return (NULL);
	line = NULL;
	line = syntax(token, &error);
	if (line)
	{
		error_hrdc(token, error);
		ft_putstr_fd(line, STDERR_FILENO);
		//return (free (line), free_token(token), NULL);
		return (NULL);
	}
	tree_planting(token);
	//token_printer(token);
	// return (free_token(token), tree);
	return (NULL);
}
