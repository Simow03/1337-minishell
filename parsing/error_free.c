/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 15:15:38 by ayyassif          #+#    #+#             */
/*   Updated: 2024/07/18 14:23:15 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//error types:
//0:	bash: syntax error near unexpected token '%s'
/*1:	bash: unexpected EOF while looking for matching `"'
		bash: syntax error: unexpected end of file	*/
//2:	bash: syntax error: unexpected end of file
//3:	bash: %s: ambiguous redirect

char	*error_printer(int err_type, char *err_msg)
{
	char	*str;
	char	*tmp;

	if (err_type == 0)
	{
		str = mergejoin("minishell: syntax error near unexpected token `",
				err_msg);
		tmp = mergejoin(str, "\'\n");
	}
	if (err_type == 1)
	{
		str = "minishell: unexpected EOF while looking for matching `";
		str = mergejoin(str, err_msg);
		tmp = mergejoin(str,
				"\'\nminishell: syntax error: unexpected end of file\n");
	}
	if (err_type == 2)
		return (ft_strdup("minishell: syntax error: unexpected end of file\n"));
	if (err_type == 3)
	{
		str = mergejoin("minishell: ", err_msg);
		tmp = mergejoin(str, ": ambiguous redirect\n");
	}
	return (free(str), tmp);
}

int	amb_error(t_token **prev, t_token *token, char *old_content)
{
	char		*err_msg;
	static int	is_full;

	is_full = 1;
	if (*prev && (*prev)->token_type != TK_REDIR_FILE)
		is_full = 0;
	while (token && token->token_type == TK_REDIR_FILE)
	{
		if (token->quote != NOT_Q || (token->content
				&& (token->content[0] != '$'
					|| value_fetcher(token->content + 1, NULL))))
			is_full = 1;
		token = token->next;
	}
	if (token && token->token_type == TK_SPACE && token->next
		&& token->next->token_type == TK_REDIR_FILE)
		is_full = 0;
	if (is_full)
		return (0);
	err_msg = error_printer(3, old_content);
	ft_putstr_fd(err_msg, STDERR_FILENO);
	free(err_msg);
	return (1);
}

void	error_hrdc(t_token *token, int pos)
{
	t_token	*next;

	pos--;
	next = token->next;
	if (next && next->token_type == TK_SPACE)
		next = next->next;
	while (token && next && --pos)
	{
		if (token->token_type == TK_HERE_DOC)
			free(here_doc_handler(next));
		token = next;
		next = next->next;
		if (next && next->token_type == TK_SPACE)
			next = next->next;
	}
}

void	free_tree(t_tree *tree)
{
	int	i;

	if (!tree)
		return ;
	free_tree(tree->left);
	free_tree(tree->right);
	i = -1;
	if (tree->node_type == TR_COMMAND)
		while (((char **)tree->content)[++i])
			free(((char **)tree->content)[i]);
	free(tree->content);
	free(tree);
}

void	free_token(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		free(token->content);
		free(token);
		token = tmp;
	}
}
