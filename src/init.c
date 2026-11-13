#include "codexion.h"

void	init_values(t_system *sys)
{
	sys->num_coders = 5;
	sys->time_to_burnout = 800;
	sys->time_to_compile = 200;
	sys->time_to_debug = 100;
	sys->time_to_refactor = 100;
	sys->dongle_cooldown = 50;
	sys->number_of_compiles_required = 7;
	sys->stop = 0;
	sys->scheduler = "fifo";
	sys->start_time = get_time();
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