/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianda <adrianda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 21:38:09 by adrianda          #+#    #+#             */
/*   Updated: 2026/11/13 17:47:22 by adrianda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_dongle		t_dongle;
typedef struct s_coder		t_coder;
typedef struct s_system		t_system;
typedef struct s_request	t_request;

typedef struct s_request
{
	int			coder_id;
	long long	key;
}				t_request;

typedef struct s_dongle
{
	int				id;
	int				in_use;
	long long		last_release_time;
	t_request		queue[2];
	int				queue_size;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
}					t_dongle;

typedef struct s_coder
{
	int				id;
	int				compile_count;
	long long		last_compile_time;
	t_dongle		*dongle_1;
	t_dongle		*dongle_2;
	pthread_mutex_t	coder_lock;
	pthread_t		coder_thread;
	t_system		*sys;
}					t_coder;

typedef struct s_system
{
	int				num_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				dongle_cooldown;
	int				stop;
	int				number_of_compiles_required;
	long long		start_time;
	char			*scheduler;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	print_lock;
	pthread_t		monitoring_thread;
}					t_system;

/* utils */
void		*ft_calloc(size_t nmemb, size_t size);
long long	get_time(void);
void		print_action(t_coder *coder, char *action);

/* queue */
void		queue_push(t_dongle *dongle, int coder_id, long long key);
void		queue_pop(t_dongle *dongle);
int			comes_first(t_request a, t_request b);

/* stop */
int			check_stop(t_system *sys);
void		set_stop(t_system *sys);
void		wake_all(t_system *sys);

/* monitor */
int			check_burnout(t_coder *coder);
int			check_all_burnout(t_system *sys);
int			check_compiles(t_coder *coder);
int			all_compiled(t_system *sys);
void		*monitor_routine(void *payload);

/* dongles */
void		enter_queue(t_coder *coder, t_dongle *dongle, long long key);
void		order_dongles(t_coder *coder, t_dongle **first, t_dongle **second);
int			dongle_ready(t_coder *coder, t_dongle *dongle);
int			try_take(t_coder *coder);
void		take_dongles(t_coder *coder);
void		free_dongle(t_dongle *dongle);
void		release_dongles(t_coder *coder);

/* coder */
void		compile(t_coder *coder);
void		debug(t_coder *coder);
void		refactor(t_coder *coder);
void		*coder_routine(void *payload);

/* main */
int			start_threads(t_system *sys);
void		stop_threads(t_system *sys);
void		init_coders(t_system *sys);
int			init_system(t_system *sys);

/* parser */
int			validate_args(int argc, char **argv);
int			parse_args(t_system *sys, char **argv);
#endif