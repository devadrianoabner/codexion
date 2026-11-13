#include "codexion.h"

void	enter_queue(t_coder *coder, t_dongle *dongle, long long key)
{
	pthread_mutex_lock(&dongle->lock);
	queue_push(dongle, coder->id, key);
	pthread_mutex_unlock(&dongle->lock);
}

void	order_dongles(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->dongle_1->id < coder->dongle_2->id)
	{
		*first = coder->dongle_1;
		*second = coder->dongle_2;
	}
	else
	{
		*first = coder->dongle_2;
		*second = coder->dongle_1;
	}
}

int	dongle_ready(t_coder *coder, t_dongle *dongle)
{
	if (dongle->queue[0].coder_id != coder->id)
		return (0);
	if (dongle->in_use)
		return (0);
	if (get_time() < dongle->last_release_time
		+ coder->sys->dongle_cooldown)
		return (0);
	return (1);
}

int	try_take(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	int			ok;

	order_dongles(coder, &first, &second);
	pthread_mutex_lock(&first->lock);
	pthread_mutex_lock(&second->lock);
	ok = dongle_ready(coder, first) && dongle_ready(coder, second);
	if (ok)
	{
		queue_pop(first);
		queue_pop(second);
		first->in_use = 1;
		second->in_use = 1;
	}
	pthread_mutex_unlock(&second->lock);
	pthread_mutex_unlock(&first->lock);
	return (ok);
}

void	take_dongles(t_coder *coder)
{
	long long	key;

	if (strcmp(coder->sys->scheduler, "edf") == 0)
		key = coder->last_compile_time + coder->sys->time_to_burnout;
	else
		key = get_time();
	enter_queue(coder, coder->dongle_1, key);
	enter_queue(coder, coder->dongle_2, key);
	while (!check_stop(coder->sys))
	{
		if (try_take(coder))
		{
			print_action(coder, "has taken a dongle");
			print_action(coder, "has taken a dongle");
			return ;
		}
		usleep(1000);
	}
}