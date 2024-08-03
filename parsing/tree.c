/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 15:54:53 by ayyassif          #+#    #+#             */
/*   Updated: 2024/08/03 16:37:15 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*merge_text(t_token **token, t_etoken token_type)
{
	char	*tmp;
	char	*str;

	str = NULL;
	tmp = NULL;
	while (*token && (*token)->token_type == token_type)
	{
		str = mergejoin(tmp, (*token)->content);
		free(tmp);
		if (!str)
			return (NULL);
		tmp = str;
		(*token) = (*token)->next;
	}
	return (str);
}

static int	tree_handler(t_tree	**tree, t_tree *new)
{
	t_tree	*tmp;

	tmp = *tree;
	if (!new || !new->content)
		return (EXIT_FAILURE);
	if (!(*tree))
	{
		*tree = new;
		return (EXIT_SUCCESS);
	}
	while (tmp->left)
		tmp = tmp->left;
	tmp->left = new;
	return (EXIT_SUCCESS);
}

static t_tree	*tree_branches(t_token *token)
{
	int		i;
	t_tree	*tree;
	char	**cmd;

	cmd = (char **)malloc(sizeof(char *) * (cmd_size(token) + 1));
	if (!cmd)
		return (NULL);
	i = 0;
	tree = NULL;
	while (token && token->token_type != TK_PIPE)
	{
		if (token->token_type == TK_COMMAND && has_content(token))
			cmd[i++] = merge_text(&token, TK_COMMAND);
		else if (token->token_type == TK_REDIR_IN
			|| token->token_type == TK_REDIR_OUT
			|| token->token_type == TK_REDIR_APND
			|| token->token_type == TK_HERE_DOC)
			tree_handler(&tree, redir_tree(&token));
		else if (token && token->token_type != TK_PIPE)
			token = token->next;
	}
	cmd[i] = NULL;
	tree_handler(&tree, cmd_tree(cmd));
	return (pipe_tree(token, tree));
}

t_tree	*pipe_tree(t_token *token, t_tree *tree)
{
	t_tree	*new;

	new = NULL;
	if (token && token->token_type == TK_PIPE)
	{
		new = (t_tree *)malloc(sizeof(t_tree));
		if (!new)
			return (perror("malloc"), NULL);
		new->node_type = TR_PIPE;
		new->left = tree;
		new->right = tree_branches(token->next);
		new->content = NULL;
		return (new);
	}
	return (tree);
}

t_tree	*tree_planting(t_token *token)
{
	t_tree	*tree;

	tree = NULL;
	token = cmd_join(token);
	if (!token)
		return (NULL);
	tree = tree_branches(token);
	free_token(token);
	return (tree);
}
