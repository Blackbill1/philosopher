/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dref <tle-dref@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:51:11 by tle-dref          #+#    #+#             */
/*   Updated: 2024/11/13 19:11:27 by tle-dref         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	args_are_correct(int ac, char **av)
{
	int	i;

	i = 1;
	if (ac != 5 && ac != 6)
		return (ft_error("Numbers of arguments diff than 5 or 6\n"), 0);
	while (i < ac)
	{
		if (!is_positive_number(av[i]))
			return (ft_error("Arguments must be positive integers\n"), 0);
		if (ft_atoi(av[i]) <= 0)
			return (ft_error("Arguments must be greater than 0\n"), 0);
		i++;
	}
	return (1);
}

int	init_params(t_params *params, int ac, char **av)
{
	if (pthread_mutex_init(&params->print_mutex, NULL) != 0
		|| pthread_mutex_init(&params->update_mutex, NULL) != 0)
		return (ft_error("Mutex initialization failed\n"), 0);
	params->nb_ph = ft_atoi(av[1]);
	params->die_t = ft_atoi(av[2]);
	params->eat_t = ft_atoi(av[3]);
	params->sleep_t = ft_atoi(av[4]);
	params->start_t = get_current_time_in_ms();
	params->simulation_end = 0;
	if (ac == 6)
		params->nb_ph_must_eat = ft_atoi(av[5]);
	else
		params->nb_ph_must_eat = -1;
	return (1);
}

t_philo	*init_philo(t_params *params)
{
	t_philo	*philos;
	int		i;

	philos = malloc(sizeof(t_philo) * params->nb_ph);
	if (!philos)
		return (ft_error("Memory allocation failed\n"), NULL);
	i = 0;
	while (i < params->nb_ph)
	{
		philos[i].id = i + 1;
		philos[i].nb_eat = 0;
		philos[i].last_eat_t = params->start_t;
		philos[i].params = params;
		if (pthread_mutex_init(&philos[i].left_fork, NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&philos[i].left_fork);
			return (ft_error("Mutex init failed\n"), free(philos), NULL);
		}
		if (i++ == params->nb_ph - 1)
			philos[i - 1].right_fork = &philos[0].left_fork;
		else
			philos[i - 1].right_fork = &philos[1 + i - 1].left_fork;
	}
	return (philos);
}
