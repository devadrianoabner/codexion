/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianda <adrianda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 17:41:38 by adrianda          #+#    #+#             */
/*   Updated: 2026/11/13 17:41:39 by adrianda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	comes_first(t_request a, t_request b)
{
	if (a.key != b.key)
		return (a.key < b.key);
	return (a.coder_id < b.coder_id);
}

void	queue_push(t_dongle *dongle, int coder_id, long long key)
{
	int			i;
	t_request	tmp;

	i = dongle->queue_size;
	dongle->queue[i].coder_id = coder_id;
	dongle->queue[i].key = key;
	dongle->queue_size++;
	if (i == 1 && comes_first(dongle->queue[1], dongle->queue[0]))
	{
		tmp = dongle->queue[0];
		dongle->queue[0] = dongle->queue[1];
		dongle->queue[1] = tmp;
	}
}

void	queue_pop(t_dongle *dongle)
{
	if (dongle->queue_size == 2)
		dongle->queue[0] = dongle->queue[1];
	dongle->queue_size--;
}
