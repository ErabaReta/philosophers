/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 21:08:16 by eouhrich          #+#    #+#             */
/*   Updated: 2024/09/05 00:48:12 by eouhrich         ###   ########.fr       */
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

void	*routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	pthread_mutex_lock(&(philo->vars->start_lock));
	pthread_mutex_unlock(&(philo->vars->start_lock));
	if (((philo->id % 2) == 1)
		|| (philo->id == (philo->vars->number_of_philosophers - 1)))
		usleep(3000);
	while (!is_finished(philo))
	{
		thinking(philo);
		eating(philo);
		sleeping(philo);
	}
	return (philo);
}
