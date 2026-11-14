/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianda <adrianda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 17:41:16 by adrianda          #+#    #+#             */
/*   Updated: 2026/11/13 20:48:50 by adrianda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_system(t_system *sys)
{
	int	i;

	i = 0;
	while (i < sys->num_coders)
	{
		pthread_mutex_destroy(&sys->dongles[i].lock);
		pthread_cond_destroy(&sys->dongles[i].cond);
		pthread_mutex_destroy(&sys->coders[i].coder_lock);
		i++;
	}
	pthread_mutex_destroy(&sys->stop_lock);
	pthread_mutex_destroy(&sys->print_lock);
	free(sys->dongles);
	free(sys->coders);
}

void	init_coders(t_system *sys)
{
	int	i;

	i = 0;
	while (i < sys->num_coders)
	{
		sys->coders[i].id = i + 1;
		sys->dongles[i].id = i + 1;
		sys->coders[i].dongle_1 = &sys->dongles[i];
		sys->coders[i].dongle_2 = &sys->dongles[(i + 1) % sys->num_coders];
		sys->coders[i].last_compile_time = sys->start_time;
		sys->coders[i].sys = sys;
		pthread_mutex_init(&sys->dongles[i].lock, NULL);
		pthread_cond_init(&sys->dongles[i].cond, NULL);
		pthread_mutex_init(&sys->coders[i].coder_lock, NULL);
		i++;
	}
}

int	init_system(t_system *sys)
{
	pthread_mutex_init(&sys->stop_lock, NULL);
	pthread_mutex_init(&sys->print_lock, NULL);
	sys->dongles = ft_calloc(sys->num_coders, sizeof(t_dongle));
	sys->coders = ft_calloc(sys->num_coders, sizeof(t_coder));
	if (!sys->dongles || !sys->coders)
		return (1);
	init_coders(sys);
	return (0);
}
