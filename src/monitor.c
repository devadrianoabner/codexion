#include "codexion.h"

int	check_burnout(t_coder *coder)
{
	long long	last;

	pthread_mutex_lock(&coder->coder_lock);
	last = coder->last_compile_time;
	pthread_mutex_unlock(&coder->coder_lock);
	return (get_time() - last > coder->sys->time_to_burnout);
}

int	check_all_burnout(t_system *sys)
{
	int	i;

	i = 0;
	while (i < sys->num_coders)
	{
		if (check_burnout(&sys->coders[i]))
		{
			print_action(&sys->coders[i], "burned out");
			set_stop(sys);
			wake_all(sys);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_compiles(t_coder *coder)
{
	int	done;

	pthread_mutex_lock(&coder->coder_lock);
	if (coder->compile_count >= coder->sys->number_of_compiles_required)
		done = 1;
	else
		done = 0;
	pthread_mutex_unlock(&coder->coder_lock);
	return (done);
}

int	all_compiled(t_system *sys)
{
	int	i;

	i = 0;
	while (i < sys->num_coders)
	{
		if (!check_compiles(&sys->coders[i]))
			return (0);
		i++;
	}
	return (1);
}

void	*monitor_routine(void *payload)
{
	t_system	*sys;

	sys = (t_system *)payload;
	while (!check_stop(sys))
	{
		if (check_all_burnout(sys))
			return (NULL);
		if (all_compiled(sys))
		{
			set_stop(sys);
			wake_all(sys);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}