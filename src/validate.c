/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianda <adrianda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 17:49:58 by adrianda          #+#    #+#             */
/*   Updated: 2026/11/13 17:51:16 by adrianda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_number(char *s)
{
	int	i;

	i = 0;
	if (!s[0])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	fits_int(char *s)
{
	long long	n;
	int			i;

	n = 0;
	i = 0;
	while (s[i])
	{
		n = n * 10 + (s[i] - '0');
		if (n > 2147483647)
			return (0);
		i++;
	}
	return (1);
}

static int	print_error(void)
{
	fprintf(stderr, "Error: invalid arguments\n");
	fprintf(stderr, "Usage: ./codexion number_of_coders time_to_burnout "
		"time_to_compile time_to_debug time_to_refactor "
		"number_of_compiles_required dongle_cooldown scheduler\n");
	return (1);
}

int	validate_args(int argc, char **argv)
{
	int	i;

	if (argc != 9)
		return (print_error());
	i = 1;
	while (i < 8)
	{
		if (!is_number(argv[i]) || !fits_int(argv[i]))
			return (print_error());
		i++;
	}
	if (atoi(argv[1]) < 1)
		return (print_error());
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
		return (print_error());
	return (0);
}
