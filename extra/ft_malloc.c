/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 13:52:00 by ayyassif          #+#    #+#             */
/*   Updated: 2024/07/01 15:31:57 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


t_alloc	*ft_last(t_alloc *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_alloc **lst, t_alloc *new)
{
	if (!(*lst))
		*lst = new;
	else
		ft_last(*lst)->next = new;
}

void	free_malloc(t_alloc *lst_alloced)
{
	t_alloc			*tmp;
	
	while (lst_alloced)
	{
		tmp = lst_alloced->next;
		free(lst_alloced->ptr);
		free(lst_alloced);
		lst_alloced = tmp;
	}
}

t_alloc	*ft_lst_new(void *ptr)
{
	t_alloc	*new;

	new = (t_alloc *)malloc(sizeof(t_alloc));
	if (!new)
	{
		perror("malloc");
		return (NULL);
	}
	new->ptr = ptr;
	new->next = NULL;
	return (NULL);
}

//0:	setter
//1:	free

void	*ft_malloc(size_t size, int mode)
{
	static t_alloc	*lst_alloced;
	t_alloc			*tmp;
	void			*ptr;
	
	if (!mode)
	{
		ptr = malloc(size);
		if (!ptr)
		{
			free_malloc(lst_alloced);
			perror("malloc");
			return (NULL);
		}
		ft_lstadd_back(&lst_alloced, ft_lst_new(ptr));
		return (ptr);
	}
	else
		free_malloc(lst_alloced);
	return (NULL);
}
