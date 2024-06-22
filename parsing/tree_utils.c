/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 15:49:58 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/22 16:32:41 by ayyassif         ###   ########.fr       */
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
	if (token->token_type == TK_REDIR_OUT)
		new->node_type = TR_REDIR_OUT;
	if (token->token_type == TK_REDIR_APND)
		new->node_type = TR_REDIR_APND;
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

t_tree	*hrdc_tree(t_token *token)
{
	t_tree		*new;

	new = (t_tree *)malloc(sizeof(t_tree));
	if (!new)
		return (perror("malloc"), NULL);
	new->node_type = TR_HERE_DOC;
	new->content = (void *)here_doc_handler(token->next);
	new->left = NULL;
	new->right = NULL;
	return (new);
}

t_token	*cmd_join_util(t_token **prev, t_token *token)
{
	static char	*old_content;
	int			i;

	if (token->token_type == TK_REDIR_FILE
		&& *prev && (*prev)->token_type != TK_REDIR_FILE)
	{
		free(old_content);
		old_content = old_str(token);
	}
	token = cmd_handlers(token, prev);
	if (token->token_type == TK_REDIR_FILE && !amb_error(prev, token, old_content))
		return (NULL);
	i = -1;
	while (token->token_type == TK_REDIR_FILE
		&& token->quote == NOT_Q && token->content && token->content[++i])
	{
		if (token->content[i] == ' ')
			return (amb_error(NULL, NULL, old_content));
	}
	if (!token->next)
	{
		free(old_content);
		old_content = NULL;
	}
	return (token);
}