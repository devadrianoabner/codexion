/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianda <adrianda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 17:41:31 by adrianda          #+#    #+#             */
/*   Updated: 2026/11/13 17:41:32 by adrianda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	start_threads(t_system *sys)
{
	int	i;

	i = 0;
	while (i < sys->num_coders)
	{
		pthread_create(&sys->coders[i].coder_thread, NULL,
			&coder_routine, &sys->coders[i]);
		i++;
	}
	pthread_create(&sys->monitoring_thread, NULL, &monitor_routine, sys);
	return (0);
}

void	stop_threads(t_system *sys)
{
	int	i;

	i = 0;
	while (i < sys->num_coders)
	{
		pthread_join(sys->coders[i].coder_thread, NULL);
		i++;
	}
}

int	main(void)
{
	t_system	sys;

	init_values(&sys);
	if (init_system(&sys))
		return (1);
	start_threads(&sys);
	stop_threads(&sys);
	pthread_join(sys.monitoring_thread, NULL);
	return (0);
}
