/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dref <tle-dref@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:00:14 by tle-dref          #+#    #+#             */
/*   Updated: 2024/11/13 19:17:11 by tle-dref         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	if (should_terminate(philo))
		return ;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
}

void	put_down_forks(t_philo *philo)
{
	print_status(philo, "has put down fork");
	pthread_mutex_unlock(philo->right_fork);
	print_status(philo, "has put down fork");
	pthread_mutex_unlock(&philo->left_fork);
}

void	try_to_eat(t_philo *philo)
{
	if (should_terminate(philo))
		return ;
	if (philo->params->nb_ph == 1)
	{
		alone(philo);
		return ;
	}
	take_forks(philo);
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->params->update_mutex);
	philo->last_eat_t = get_current_time_in_ms() + philo->params->eat_t;
	philo->nb_eat++;
	if (philo->params->nb_ph_must_eat != -1
		&& philo->nb_eat >= philo->params->nb_ph_must_eat)
	{
		philo->params->stomach_full++;
		philo->nb_eat = -1;
		if (philo->params->stomach_full == philo->params->nb_ph)
			philo->params->simulation_end = 1;
	}
	pthread_mutex_unlock(&philo->params->update_mutex);
	usleep(philo->params->eat_t * 1000);
	put_down_forks(philo);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep((philo->params->eat_t / 2) * 1000);
	while (1)
	{
		if (should_terminate(philo) || philo->nb_eat == -1)
			break ;
		print_status(philo, "is thinking");
		try_to_eat(philo);
		print_status(philo, "is sleeping");
		usleep(philo->params->sleep_t * 1000);
	}
	return (NULL);
}

void	alone(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork);
	print_status(philo, "has taken a fork");
	usleep(philo->params->die_t * 1000);
	pthread_mutex_unlock(&philo->left_fork);
	pthread_mutex_lock(&philo->params->update_mutex);
	philo->params->simulation_end = 1;
	pthread_mutex_unlock(&philo->params->update_mutex);
	return ;
}
