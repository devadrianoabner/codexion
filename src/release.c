#include "codexion.h"

void	free_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->in_use = 0;
	dongle->last_release_time = get_time();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}

void	release_dongles(t_coder *coder)
{
	free_dongle(coder->dongle_1);
	free_dongle(coder->dongle_2);
}
