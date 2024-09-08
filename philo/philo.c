/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 21:08:16 by eouhrich          #+#    #+#             */
/*   Updated: 2024/09/08 21:39:38 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	thinking(t_philo *philo)
{
	pthread_mutex_lock(&(philo->think_lock));
	philo->think_logged = 0;
	pthread_mutex_unlock(&(philo->think_lock));
}

void	sleeping(t_philo *philo)
{
	pthread_mutex_lock(&(philo->sleep_lock));
	philo->sleep_logged = 0;
	pthread_mutex_unlock(&(philo->sleep_lock));
	wait_for(philo, philo->vars->time_to_sleep);
}

void	think_till_hungry(t_philo *philo)
{
	thinking(philo);
	pthread_mutex_lock(&(philo->last_eat_lock));
	while (((get_time_milliseconds(philo->vars->tv) - philo->last_eat)
			< (philo->vars->time_to_die - 15)) && !is_finished(philo))
	{
		pthread_mutex_unlock(&(philo->last_eat_lock));
		usleep(10);
		pthread_mutex_lock(&(philo->last_eat_lock));
	}
	pthread_mutex_unlock(&(philo->last_eat_lock));
}

void	*routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	pthread_mutex_lock(&(philo->vars->start_lock));
	pthread_mutex_unlock(&(philo->vars->start_lock));
	pthread_mutex_lock(&(philo->last_eat_lock));
	philo->last_eat = get_time_milliseconds((philo)->tv);
	pthread_mutex_unlock(&(philo->last_eat_lock));
	if (((philo->id % 2) == 0) && philo->vars->time_to_eat >= 10)
		wait_for(philo, philo->vars->time_to_eat - 10);
	if (philo->id == philo->vars->number_of_philosophers
		&& philo->vars->number_of_philosophers % 2 == 1
		&& philo->vars->time_to_eat >= 5)
		wait_for(philo, (philo->vars->time_to_eat * 2) - 10);
	while (!is_finished(philo))
	{
		eating(philo);
		sleeping(philo);
		think_till_hungry(philo);
	}
	return (philo);
}
