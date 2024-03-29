/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlowe <nlowe@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/11 21:53:36 by nlowe             #+#    #+#             */
/*   Updated: 2017/05/29 17:50:09 by nlowe            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

extern unsigned char g_options;
extern unsigned char g_sort;

void	free_entry(t_entry **entry)
{
	free((*entry)->path);
	if ((*entry)->name)
		free((*entry)->name);
	free((*entry)->stats);
	free(*entry);
	(*entry) = NULL;
}

void	free_list(t_entry **start)
{
	t_entry		*temp;
	t_entry		*trash;

	if (!start || !(*start))
		return ;
	temp = *start;
	while (temp)
	{
		trash = temp;
		temp = temp->next;
		free_entry(&trash);
	}
}

t_entry	*new_entry(void)
{
	t_entry	*ret;

	if (!(ret = (t_entry *)malloc(sizeof(t_entry))))
		return (NULL);
	if (!(ret->path = ft_strnew(PATH_MAX + 1)))
		return (NULL);
	if (!(ret->stats = (t_stat *)malloc(sizeof(t_stat))))
		return (NULL);
	ret->next = NULL;
	return (ret);
}

void	entry_addby(t_entry **alst, t_entry *new,
	int (*cmp)(t_entry *, t_entry *))
{
	t_entry		*current;
	t_entry		*previous;

	current = *alst;
	previous = NULL;
	while ((current != NULL) && (cmp(new, current) >= 0))
	{
		previous = current;
		current = current->next;
	}
	if (previous != NULL)
	{
		new->next = previous->next;
		previous->next = new;
	}
	else
	{
		new->next = *alst;
		*alst = new;
	}
}

void	ft_insert(t_entry **start, t_entry *new)
{
	int		(*cmp)(t_entry *, t_entry *);

	if (!start || !new)
		return ;
	if (g_sort & SORT_MOD || g_sort & SORT_ACC || g_sort & SORT_SC)
		cmp = compare_time;
	else if (g_sort & SORT_SIZE)
		cmp = compare_size;
	else if (g_sort & SORT_NONE)
		cmp = none;
	else
		cmp = compare_path;
	entry_addby(start, new, cmp);
}
