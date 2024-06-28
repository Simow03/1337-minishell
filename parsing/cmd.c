// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   tree.c                                             :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/05/06 14:33:10 by ayyassif          #+#    #+#             */
// /*   Updated: 2024/06/14 17:49:31 by ayyassif         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "../minishell.h"

int	cmd_size(t_token *token)
{
	int	size;
	int	is_full;

	size = 0;
	while (token && token->token_type != TK_PIPE)
	{
		is_full = 1;
		if (token->token_type == TK_COMMAND)
		{
			is_full = 0;
			size++;
		}
		while (token->token_type == TK_COMMAND
			&& token->next && token->next->token_type == TK_COMMAND)
		{
			if (token->content)
				is_full = 1;
			token  = token->next;
		}
		if (!token->content && is_full == 0)
			size--;
		if (token && token->token_type != TK_PIPE)
			token = token->next;
	}
	return (size);
}

t_tree	*cmd_tree(char	**cmd)
{
	t_tree		*new;

	if (!cmd[0])
	{
		free(cmd);
		return (NULL);
	}
	new = (t_tree *)malloc(sizeof(t_tree));
	if (!new)
		return (perror("malloc"), NULL);
	new->left = NULL;
	new->right = NULL;
	new->node_type = TR_COMMAND;
	new->content = (void *)cmd;
	return (new);
}
