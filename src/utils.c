/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianda <adrianda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 17:41:47 by adrianda          #+#    #+#             */
/*   Updated: 2026/11/13 17:41:48 by adrianda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	total;

	if (nmemb != 0 && (nmemb * size) / nmemb != size)
		return (NULL);
	total = nmemb * size;
	ptr = malloc(total);
	if (!ptr)
		return (NULL);
	memset(ptr, 0, total);
	return (ptr);
}

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	print_action(t_coder *coder, char *action)
{
	long long	current_time;

	if (check_stop(coder->sys))
		return ;
	pthread_mutex_lock(&coder->sys->print_lock);
	current_time = get_time() - coder->sys->start_time;
	printf("%lld %d %s\n", current_time, coder->id, action);
	pthread_mutex_unlock(&coder->sys->print_lock);
}
