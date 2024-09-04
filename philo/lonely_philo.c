/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lonely_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 14:58:33 by eouhrich          #+#    #+#             */
/*   Updated: 2024/09/05 00:48:54 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*lonely_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	pthread_mutex_lock(&(philo->vars->start_lock));
	pthread_mutex_unlock(&(philo->vars->start_lock));
	pthread_mutex_lock(&(philo->think_lock));
	philo->think_logged = 0;
	pthread_mutex_unlock(&(philo->think_lock));
	pthread_mutex_lock(&philo->vars->forks[0]);
	pthread_mutex_lock(&(philo->fork_lock));
	philo->fork_logged = 0;
	pthread_mutex_unlock(&(philo->fork_lock));
	while (!is_finished(philo))
		wait_for(philo, 1);
	pthread_mutex_unlock(&philo->vars->forks[0]);
	return (philo);
}

int	create_lonely_philo(t_vars *vars, t_philo **philo, pthread_t	*watcher)
{
	if (pthread_create(&(vars->philos[0]), NULL, lonely_routine, philo[0]) != 0)
	{
		printf("failed to create philo thread\n");
		return (-1);
	}
	if (pthread_create(watcher, NULL, watching, philo) != 0)
	{
		printf("failed to create watcher thread\n");
		return (-1);
	}
	vars->initial_timeval = get_time_milliseconds((*philo)->tv);
	philo[0]->last_eat = get_time_milliseconds((*philo)->tv);
	return (0);
}
