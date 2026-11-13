/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianda <adrianda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 17:41:26 by adrianda          #+#    #+#             */
/*   Updated: 2026/11/13 19:39:06 by adrianda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_lock);
	coder->last_compile_time = get_time();
	coder->compile_count++;
	pthread_mutex_unlock(&coder->coder_lock);
	print_action(coder, "is compiling");
	usleep(coder->sys->time_to_compile * 1000);
}

void	debug(t_coder *coder)
{
	print_action(coder, "is debugging");
	usleep(coder->sys->time_to_debug * 1000);
}

void	refactor(t_coder *coder)
{
	print_action(coder, "is refactoring");
	usleep(coder->sys->time_to_refactor * 1000);
}

void	*coder_routine(void *payload)
{
	t_coder	*coder;

	coder = (t_coder *)payload;
	if (coder->id % 2 == 0)
		usleep(coder->sys->time_to_compile * 500);
	while (!check_stop(coder->sys))
	{
		take_dongles(coder);
		compile(coder);
		release_dongles(coder);
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}
