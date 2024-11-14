/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dref <tle-dref@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:06:16 by tle-dref          #+#    #+#             */
/*   Updated: 2024/11/14 18:03:55 by tle-dref         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	should_terminate(t_philo *philo)
{
	int	end;

	pthread_mutex_lock(&philo->params->update_mutex);
	end = philo->params->simulation_end;
	pthread_mutex_unlock(&philo->params->update_mutex);
	return (end);
}

void	destroy_mutexes(t_philo *philos)
{
	int	i;
	int	nb_ph;

	nb_ph = philos[0].params->nb_ph;
	i = 0;
	while (i < nb_ph)
	{
		pthread_mutex_destroy(&philos[i].left_fork);
		i++;
	}
	pthread_mutex_destroy(&philos[0].params->print_mutex);
	pthread_mutex_destroy(&philos[0].params->update_mutex);
}

void	wait_threads(t_philo *philos)
{
	int	i;
	int	nb_ph;

	nb_ph = philos[0].params->nb_ph;
	i = 0;
	while (i < nb_ph)
	{
		if (pthread_join(philos[i].thread, NULL) != 0)
		{
			printf("Failed to join thread\n");
			return ;
		}
		i++;
	}
}

void	*monitor_all(void *arg)
{
	t_philo	*philos;
	int		i;

	philos = (t_philo *)arg;
	while (1)
	{
		i = 0;
		while (i < philos[0].params->nb_ph)
		{
			if (should_terminate(&philos[i]))
				return (NULL);
			pthread_mutex_lock(&philos[i].params->update_mutex);
			if (get_current_time_in_ms() > philos[i].last_eat_t
				+ philos[i].params->die_t)
			{
				philos[i].params->simulation_end = 1;
				pthread_mutex_unlock(&philos[i].params->update_mutex);
				print_status(&philos[i], "died");
				return (NULL);
			}
			pthread_mutex_unlock(&philos[i].params->update_mutex);
			i++;
		}
	}
	return (NULL);
}

void	create_threads(t_philo *philos)
{
	int	i;
	int	nb_ph;

	nb_ph = philos[0].params->nb_ph;
	i = 0;
	while (i < nb_ph)
	{
		if (pthread_create(&philos[i].thread, NULL, &routine, &philos[i]) != 0)
		{
			printf("Failed to create thread\n");
			return ;
		}
		i++;
	}
}
