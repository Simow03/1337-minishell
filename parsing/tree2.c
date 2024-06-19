/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 15:54:53 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/19 17:08:14 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*value_fetcher(char *text, int *size)
{
	size_t	i;
	t_env	*env;

	env = global_env(NULL, 0);
	i = 0;
	while (ft_isalpha(text[i]) || text[i] == '_')
		i++;
	if (size)
		*size = i;
	if (!i && text[i] == '?')
		return (global_return_str(0, 0));
	if (!i)
		return (NULL);
	while (env)
	{
		if (ft_strlen(env->name) == i && !ft_strncmp(text, env->name, i))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

t_token	*quote_expend(char *str, t_token *next, t_etoken token_type)
{
	t_token	*new;
	int		size;

	size = 0;
	if (str[0] == '\"')
		return (next);
	new = (t_token *)malloc(sizeof(t_token));
	new->token_type = token_type;
	new->quote = DOUBLE_Q;
	while (str[size] != '\"' && str[size] != '$')
		size++;
	if (*str == '$')
		new->content = value_fetcher(++str, &size);
	else
		new->content = ft_substr(str, 0, size);
	new->next = quote_expend(str + size, next, token_type);
	return (new);
}

t_token	*cmd_handlers(t_token *token, t_token **prev)
{
	t_token	*t_tmp;

	if (token->token_type == TK_COMMAND || token->token_type == TK_REDIR_FILE)
	{
		if (token->quote == NOT_Q && token->content && token->content[0] == '$')
			token->content = ft_strdup(value_fetcher(token->content + 1, NULL));
		else if (token->quote == SINGLE_Q)
			token->content = ft_substr(token->content, 1, ft_strlen(token->content) - 2);
		else if (token->quote == DOUBLE_Q)
		{
			t_tmp = token;
			token = quote_expend(token->content + 1, token->next, token->token_type);
			free(t_tmp);
			if (*prev)
				(*prev)->next = token;
		}
	}
	return (token);
}

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

t_token	*amb_error(t_token **prev, t_token *token, char *old_content)
{
	char		*err_msg;
	static int	is_full;

	if (token)
	{
		if (*prev && (*prev)->token_type != TK_REDIR_FILE)
			is_full = 0;
		if (token->content)
			is_full = 1;
		if (((!token->next || token->next->token_type != TK_REDIR_FILE) && is_full) ||
			(token->next && token->next->token_type == TK_REDIR_FILE))
			return (token);
	}
	err_msg = error_printer(3, old_content);
	ft_putstr_fd(err_msg, STDERR_FILENO);
	free(err_msg);
	return (NULL);
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

t_token	*cmd_join(t_token *token)
{
	t_token	*prev;
	t_token	*start;

	prev = NULL;
	start = token;
	while (token && token->token_type != TK_PIPE)
	{
		token = cmd_join_util(&prev, token);
		if (!token)
		{
			free(start);
			return (NULL);
		}
		if (!prev)
			start = token;
		prev = token;
		while (token && token->quote == DOUBLE_Q && token->content
			&& token->content[0] != '\"')
			token = token ->next;
		if (token)
			token = token->next;
	}
	return (start);
}

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
		if (token)
			token  = token->next;
	}
	return (size);
}

char	*merge_text(t_token **token, t_etoken token_type)
{
	char	*tmp;
	char	*str;

	str = NULL;
	tmp = NULL;
	while ((*token) && (*token)->token_type == token_type)
	{
		str = ft_strjoin(tmp, (*token)->content);
		free(tmp);
		if (!str)
			return (NULL);
		tmp = str;
		(*token) = (*token)->next;
	}
	return (str);
}

int	tree_handler(t_tree	**tree, t_tree *new)
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
	new->content = (void *)here_doc_handler(token->next, NULL);
	new->left = NULL;
	new->right = NULL;
	return (new);
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

t_tree	*tree_branches(t_token *token);

t_tree	*pipe_tree(t_token *token, t_tree *tree)
{
	t_tree	*new;

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

void	hdc_tree()
{
	
}

t_tree	*tree_branches(t_token *token)
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
		if (has_content(token))
			cmd[i++] = merge_text(&token , TK_COMMAND);
		else if (token->token_type == TK_REDIR_IN || token->token_type == TK_REDIR_OUT
			|| token->token_type == TK_REDIR_APND)
			tree_handler(&tree, redir_tree(token));
		else if (token->token_type == TK_HERE_DOC)
			tree_handler(&tree, hrdc_tree(token->next));
		if (token)
			token = token->next;
	}
	cmd[i] = NULL;
	tree_handler(&tree, cmd_tree(cmd));
	return (pipe_tree(token, tree));
}

// static void token_printer(t_token *token)
// {
// 	while (token)
// 	{
// 		printf("%d\t%s\n", token->token_type, token->content);
// 		token = token->next;
// 	}
// }

t_tree	*tree_planting(t_token *token)
{
	t_tree	*tree;

	tree = NULL;
	token = cmd_join(token);
	if (!token)
		return (NULL);
	tree = tree_branches(token);
	//token_printer(token);
	return (tree);
}
