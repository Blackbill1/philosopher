/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dref <tle-dref@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:50:40 by tle-dref          #+#    #+#             */
/*   Updated: 2024/11/13 18:56:23 by tle-dref         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_current_time_in_ms(void)
{
	struct timeval	tv;
	long			milliseconds;

	gettimeofday(&tv, NULL);
	milliseconds = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (milliseconds);
}

void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	pthread_mutex_lock(&philo->params->update_mutex);
	if (philo[0].params->simulation_end == 0 || ft_strcmp(status, "died") == 0)
	{
		pthread_mutex_lock(&philo->params->print_mutex);
		timestamp = get_current_time_in_ms() - philo->params->start_t;
		printf("%ld %d %s\n", timestamp, philo->id, status);
		pthread_mutex_unlock(&philo->params->print_mutex);
	}
	pthread_mutex_unlock(&philo->params->update_mutex);
}
