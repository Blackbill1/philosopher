/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dref <tle-dref@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:03:27 by tle-dref          #+#    #+#             */
/*   Updated: 2024/11/13 19:18:37 by tle-dref         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_params
{
	int				nb_ph;
	int				sleep_t;
	int				eat_t;
	int				die_t;
	int				nb_ph_must_eat;
	long			start_t;
	int				simulation_end;
	int				stomach_full;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	update_mutex;
}					t_params;

typedef struct s_philo
{
	int				id;
	int				nb_eat;
	long			last_eat_t;
	pthread_t		thread;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	*right_fork;
	struct s_params	*params;
}					t_philo;

int					is_positive_number(char *str);
int					ft_atoi(const char *str);
int					ft_strlen(const char *s);
void				ft_error(const char *message);
int					ft_strcmp(const char *str1, const char *str2);

int					args_are_correct(int ac, char **av);
int					init_params(t_params *params, int ac, char **av);
t_philo				*init_philo(t_params *params);

long				get_current_time_in_ms(void);
void				print_status(t_philo *philo, char *status);

void				take_forks(t_philo *philo);
void				put_down_forks(t_philo *philo);
void				try_to_eat(t_philo *philo);
void				*routine(void *arg);
void				alone(t_philo *philo);

void				*monitor_all(void *arg);
void				wait_threads(t_philo *philos);
void				destroy_mutexes(t_philo *philos);
int					should_terminate(t_philo *philo);
void				create_threads(t_philo *philos);
