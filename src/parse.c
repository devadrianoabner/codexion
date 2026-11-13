/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianda <adrianda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 17:50:01 by adrianda          #+#    #+#             */
/*   Updated: 2026/11/13 17:51:13 by adrianda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parse_args(t_system *sys, char **argv)
{
	sys->num_coders = atoi(argv[1]);
	sys->time_to_burnout = atoi(argv[2]);
	sys->time_to_compile = atoi(argv[3]);
	sys->time_to_debug = atoi(argv[4]);
	sys->time_to_refactor = atoi(argv[5]);
	sys->number_of_compiles_required = atoi(argv[6]);
	sys->dongle_cooldown = atoi(argv[7]);
	sys->scheduler = argv[8];
	sys->stop = 0;
	return (0);
}
