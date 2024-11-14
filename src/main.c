/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dref <tle-dref@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:03:15 by tle-dref          #+#    #+#             */
/*   Updated: 2024/11/14 18:01:54 by tle-dref         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_params	params;
	t_philo		*philos;
	pthread_t	monitor_thread;

	if (!args_are_correct(ac, av))
		return (1);
	if (!init_params(&params, ac, av))
		return (1);
	philos = init_philo(&params);
	if (!philos)
		return (1);
	create_threads(philos);
	if (pthread_create(&monitor_thread, NULL, &monitor_all, philos) != 0)
	{
		printf("Failed to create monitor thread\n");
		destroy_mutexes(philos);
		free(philos);
		return (1);
	}
	pthread_join(monitor_thread, NULL);
	wait_threads(philos);
	destroy_mutexes(philos);
	free(philos);
	return (0);
}
