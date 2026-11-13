/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianda <adrianda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 17:41:44 by adrianda          #+#    #+#             */
/*   Updated: 2026/11/13 17:41:45 by adrianda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_stop(t_system *sys)
{
	int	stop_status;

	pthread_mutex_lock(&sys->stop_lock);
	stop_status = sys->stop;
	pthread_mutex_unlock(&sys->stop_lock);
	return (stop_status);
}

void	set_stop(t_system *sys)
{
	pthread_mutex_lock(&sys->stop_lock);
	sys->stop = 1;
	pthread_mutex_unlock(&sys->stop_lock);
}

void	wake_all(t_system *sys)
{
	int	i;

	i = 0;
	while (i < sys->num_coders)
	{
		pthread_mutex_lock(&sys->dongles[i].lock);
		pthread_cond_broadcast(&sys->dongles[i].cond);
		pthread_mutex_unlock(&sys->dongles[i].lock);
		i++;
	}
}
