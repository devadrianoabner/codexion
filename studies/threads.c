#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	total;

	if (nmemb != 0 && (nmemb * size) / nmemb != size)
		return (NULL);
	total = nmemb * size;
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	memset(ptr, 0, total);
	return (ptr);
}

typedef struct s_dongle
{
	int				id;
	long long		last_release_time;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
}	t_dongle;


typedef struct s_coder
{
	int				id;
	long long		last_compile_time;
	t_dongle		*dongle_1;
	t_dongle		*dongle_2;
	pthread_mutex_t	coder_lock;
}					t_coder;


typedef struct s_system
{
	int				num_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				dongle_cooldown;
	char			*scheduler; // fifo ou edf
	t_dongle		*dongles;
	t_coder			*coders;
}	t_system;


int main(void)
{
	t_system	sys;
	int			i;

	sys.num_coders = 5;
	sys.time_to_burnout = 800;
	sys.time_to_compile = 200;
	sys.time_to_debug = 100;
	sys.time_to_refactor = 100;
	sys.dongle_cooldown = 50;
	sys.scheduler = "fifo";

	sys.dongles = ft_calloc(sys.num_coders, sizeof(t_dongle));
	sys.coders = ft_calloc(sys.num_coders, sizeof(t_coder));

	if (!sys.dongles || !sys.coders)
	{
		printf("Error, miss dongles or coders");
		return (0);
	}

	i = 0;

	while (i < sys.num_coders)
	{
		sys.coders[i].id = i + 1;
		sys.dongles[i].id = i + 1;

		pthread_mutex_init(&sys.dongles[i].lock, NULL);
		pthread_cond_init(&sys.dongles[i].cond, NULL);

		pthread_mutex_init(&sys.coders[i].coder_lock, NULL);
		
		i++;
	}

	return (0);
}