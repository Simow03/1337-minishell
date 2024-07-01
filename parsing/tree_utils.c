/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 15:49:58 by ayyassif          #+#    #+#             */
/*   Updated: 2024/07/01 14:07:56 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*old_str(t_token *token)
{
	char	*old_content;
	char	*tmp;

	old_content = NULL;
	tmp = NULL;
	while (token && token->token_type == TK_REDIR_FILE)
	{
		old_content = ft_strjoin(tmp, token->content);
		free(tmp);
		tmp = old_content;
		token = token->next;
	}
	return (old_content);
}

int	has_content(t_token *token)
{
	while (token && token->token_type == TK_COMMAND)
	{
		if (token->content)
			return (1);
		token = token->next;
	}
	return (0);
}

t_tree	*redir_tree(t_token *token)
{
	t_tree		*new;

	new = (t_tree *)malloc(sizeof(t_tree));
	if (!new)
		return (perror("malloc"), NULL);
	new->left = NULL;
	new->right = NULL;
	if (token->token_type == TK_REDIR_IN)
		new->node_type = TR_REDIR_IN;
	else if (token->token_type == TK_REDIR_OUT)
		new->node_type = TR_REDIR_OUT;
	else if (token->token_type == TK_REDIR_APND)
		new->node_type = TR_REDIR_APND;
	else
		new->node_type = TR_HERE_DOC;
	token = token->next;
	if (token->token_type == TK_SPACE)
		token = token->next;
	new->content = (void *)merge_text(&token, token->token_type);
	if (!new->content)
	{
		free(new);
		return (NULL);
	}
	return (new);
}
